DNSServer dnsServerAP;



// Configura el ap

void setupAP(char *ssid = SSID_AP)
{
  // Creamos la wifi

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid);

  // Habilitamos un servidor dns

  dnsServerAP.start(DNS_PORT, "*", apIP);
}

// Procesamos las peticiones que se nos pidan

void loopAP()
{
  dnsServerAP.processNextRequest();
}


// Esta función obtiene el tipo de encriptación de la red 
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

// Genera un error que será mostrado en el front

void generateError(String errorMessage)
{
  File log = SPIFFS.open("/error.json", "w");
  String error = "{\"error\": \"" + errorMessage + "\"}";
  log.write(error.c_str());
  log.close();
}

// Carga la configuración del fichero
void readConfig()
{
  File configFile = SPIFFS.open("/config.json", "r");
  deserializeJson(configuration, configFile.readString());
  configFile.close();
}

// Conecta a una red Wi-Fi
void connectWiFi()
{

  // Habilitamos el modo estación

  WiFi.mode(WIFI_STA);

  // Establecemos los datos de la configuración

  String ssid = configuration["ssid"];
  String psk = configuration["password"];

  // Iniciamos la configuración

  WiFi.begin(ssid.c_str(), psk.c_str());

  // Comrpobamos si nos podemos conectar

  long timeStart = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - timeStart) < TIME_WAIT_WIFI)
  {
    Serial.println("Conectando a red Wi-Fi ...");
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    // En caso de error generamos el error

    SPIFFS.remove("/config.json");
    generateError("No se pudo conectar a la red");

    // Reiniciamos el dispositivo

    ESP.reset();
  }
}

// Obtiene el token del usuario

void setToken()
{

  // Intenteamos conectarnos

  Serial.println("Obteniendo token ...");
  client.setInsecure();
  client.connect(URL_P_M, 443);

  // Iniciamos la configuración

  http.begin(client, URL_P_M + LOGIN);
  http.addHeader("Content-Type", "application/json");

  // Establecemos usuario y contrasñea

  String user = configuration["user"];
  String pskUser = configuration["passwordUser"];

  // Lanzamos y obtenemos el código de error

  int httpResponseCode = http.POST("{\"dni\":\"" + user + "\", \"password\":\"" + pskUser + "\"}");

  Serial.println("Respuesta: ");
  Serial.println(httpResponseCode);

  // Si no es correcto reseteamos la configuración 
  if (!(httpResponseCode == 200 || httpResponseCode == 201))
  {
    SPIFFS.remove("/config.json");

    if (httpResponseCode == 400) {

      // En caso de error leemos el error
      DynamicJsonDocument responseJSON(1024);
      deserializeJson(responseJSON, http.getString());
      String messageError = responseJSON["mensaje"];
      Serial.println(messageError.c_str());
      generateError(messageError.c_str());
    }
    else {
      
      // Generamos un error

      generateError("Servidor no disponible");
    }

    // Reiniciamos
    
    ESP.reset();
  }
  else
  {
    // Si todo ha ido correcto obtenemos el token y lo guardamos 

    DynamicJsonDocument response(1024);
    deserializeJson(response, http.getString());

    String currentToken = response["jwt_token"];

    token = currentToken.c_str();
  }
}

// Resetea el dispositivo
void resetConfig()
{
  Serial.println("Reestablecimiendo ...");

  // Borramos la configuración y el fichero de error

  SPIFFS.remove("/config.json");
  SPIFFS.remove("/error.json");

  // Reseteamos el dispositivo

  ESP.reset();
}


// Convierte la señal del dispositivo a porcentaje
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
