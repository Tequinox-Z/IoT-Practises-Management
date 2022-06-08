DNSServer dnsServerAP;

void setupAP(char *ssid = SSID_AP)
{
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid);

  dnsServerAP.start(DNS_PORT, "*", apIP);
}

void loopAP()
{
  dnsServerAP.processNextRequest();
}

String convertIntToTypeEncrypt(int wifiEncryp)
{
  String encrypt = "";

  switch (wifiEncryp)
  {
  case ENC_TYPE_WEP:
    encrypt = "WEP";
    break;
  case ENC_TYPE_TKIP:
    encrypt = "WPA/PSK";
    break;
  case ENC_TYPE_CCMP:
    encrypt = "WPA2/PSK";
    break;
  case ENC_TYPE_NONE:
    encrypt = "open";
    break;
  case ENC_TYPE_AUTO:
    encrypt = "WPA/WPA2/PSK";
    break;
  }
  return encrypt;
}

void generateError(String errorMessage)
{
  File log = SPIFFS.open("/error.json", "w");
  String error = "{\"error\": \"" + errorMessage + "\"}";
  log.write(error.c_str());
  log.close();
}

void readConfig()
{
  File configFile = SPIFFS.open("/config.json", "r");
  deserializeJson(configuration, configFile.readString());
  configFile.close();
}

void connectWiFi()
{

  WiFi.mode(WIFI_STA);

  String ssid = configuration["ssid"];
  String psk = configuration["password"];

  WiFi.begin(ssid.c_str(), psk.c_str());

  long timeStart = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - timeStart) < TIME_WAIT_WIFI)
  {
    Serial.println("Conectando a red Wi-Fi ...");
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    SPIFFS.remove("/config.json");
    generateError("Contraseña Wi-Fi incorrecta");
    ESP.reset();
  }
}

void setToken()
{
  Serial.println("Obteniendo token ...");
  client.setInsecure();
  client.connect(URL_P_M, 443);

  http.begin(client, URL_P_M + LOGIN);
  http.addHeader("Content-Type", "application/json");

  String user = configuration["user"];
  String pskUser = configuration["passwordUser"];

  int httpResponseCode = http.POST("{\"dni\":\"" + user + "\", \"password\":\"" + pskUser + "\"}");

  Serial.println("Respuesta: ");
  Serial.println(httpResponseCode);

  if (!(httpResponseCode == 200 || httpResponseCode == 201))
  {
    SPIFFS.remove("/config.json");

    if (httpResponseCode == 400) {
      DynamicJsonDocument responseJSON(1024);
      deserializeJson(responseJSON, http.getString());
      String messageError = responseJSON["mensaje"];
      Serial.println(messageError.c_str());
      generateError(messageError.c_str());
    }
    else {
      generateError("Servidor no disponible");
    }

    ESP.reset();
  }
  else
  {
    DynamicJsonDocument response(1024);
    deserializeJson(response, http.getString());

    String currentToken = response["jwt_token"];

    token = currentToken.c_str();
  }
}

void resetConfig()
{
  Serial.println("Reestablecimiendo ...");

  SPIFFS.remove("/config.json");
  SPIFFS.remove("/error.json");

  ESP.reset();
}

int dBmtoPercentage(int dBm)
{
  int quality;
  if (dBm <= RSSI_MIN)
  {
    quality = 0;
  }
  else if (dBm >= RSSI_MAX)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (dBm + 100);
  }

  return quality;
}
