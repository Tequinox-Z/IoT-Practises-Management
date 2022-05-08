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

void generateError(String errorMessage) {
      File log = SPIFFS.open("/error.json", "w");
      String error = "{\"error\": \"" + errorMessage + "\"}";
      log.write(error.c_str());
      log.close();
}

void readConfig() {
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

  while (WiFi.status() != WL_CONNECTED && (millis() - timeStart) < TIME_WAIT_WIFI) {
    Serial.println("Conectando a red Wi-Fi ...");
  }

    if (WiFi.status() != WL_CONNECTED)
    {
      SPIFFS.remove("/config.json");

      generateError("No se pudo establecer conexión con la red Wi-Fi");

      ESP.reset();
    }
}



void setToken() {

  http.begin(client, URL_P_M + LOGIN);
  http.addHeader("Content-Type", "application/json");

  String user = configuration["user"];
  String pskUser = configuration["passwordUser"];

  int httpResponseCode = http.POST("{\"dni\":\"" + user + "\", \"password\":\"" + pskUser + "\"}");

  if (httpResponseCode != 200)
  {
      SPIFFS.remove("/config.json");

      generateError("Credenciales inválidas o servidor no disponible");

      ESP.reset();
  }
  else
  {
    String configString;
    
    File configFile = SPIFFS.open("/config.json", "r");
    configString = configFile.readString();
    configFile.close();

    configString = configString.substring(0, configString.length() - 1);
    
    DynamicJsonDocument response(1024);
    deserializeJson(response, http.getString());

    String token = response["jwt_token"];

    configString = configString + ", \"token\":\"" + token + "\"}";

    File configFileWrite = SPIFFS.open("/config.json", "w");
    configFileWrite.print(configString.c_str());    
    configFileWrite.close();
  }
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
