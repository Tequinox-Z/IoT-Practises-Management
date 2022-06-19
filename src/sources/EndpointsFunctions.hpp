#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <string>
#include "user_interface.h"


// Configura el dispositivo con la contraseña Wi-Fi y credenciales del usuario
void configure()
{
  String response = s.arg("plain").c_str();
  Serial.println("Response: " + response);

  // Comprobamos la longitud del json

  if (response.length() > 1024) {
    // Si es mayor al límite devolvemos un error

    s.send(400, "application/json",  "{\"status\":\"Tamaño máximo del archivo excedido (1024 caracteres máximo)\"}");
  }

  // Creamos un objeto json

  DynamicJsonDocument doc(1024);

  // Deserializamos la respuesta

  deserializeJson(doc, response);

  // Obtenemos los datos

  String ssid = doc["ssid"];
  String password = doc["password"];
  String user = doc["user"];
  String userPsk = doc["passwordUser"];

  // Comprobamos que todos los datos sean correctos

  if (ssid.length() == 0 || user.length() == 0 || userPsk.length() == 0)
  {
    s.send(400, "application/json", "{\"status\":\"Indique todos los datos correctamente\"}");
  }
  else {
    
    // Enviamos un 201 Created
    
    s.send(201, "application/json");
    
    // Guardamos el fichero

    File configFile = SPIFFS.open("/config.json", "w");
    configFile.print(response.c_str());

    // Cerramos el fichero

    configFile.close();

    // Reiniciamos

    ESP.reset();
  }
  }

// Retorna todas las Wi-Fi que hay alrededor
void scanWifi()
{
  // Obtenemos las redes

  int networksFound = WiFi.scanNetworks();
  String jsonAPs = "{\"networks\": [";

  // Las iteramos y creamos un objeto

  for (int i = 0; i < networksFound; i++)
  {
    jsonAPs = jsonAPs + "{\"signal\":\"" + dBmtoPercentage(WiFi.RSSI(i)) + "\",";
    jsonAPs = jsonAPs + "\"ssid\":\"" + WiFi.SSID(i) + "\",";
    jsonAPs = jsonAPs + "\"encryptionType\":\"" + convertIntToTypeEncrypt(WiFi.encryptionType(i)) + "\"";
    jsonAPs = jsonAPs + "},";
  }

  // Si hay alguna red quitamos la coma del final

  if (networksFound != 0)
  {
    jsonAPs.remove(jsonAPs.length() - 1, 1);
  }

  // Añadimos el final del json

  jsonAPs += "]}";

  // Enviamos la respuesta

  s.send(200, "application/json", jsonAPs);
}

// Checkea si existe un movimiento

boolean checkMotion()
{
  return digitalRead(PIR_PIN) == HIGH;        // Retornamos el valor del pin
}

// Checkea si hay un moimiento y envia la peticion de movimiento detectado
void checkPIRsensor()
{
  if (checkMotion())
  {
    // Si hay movimiento parpadeamos las luces

    for (int i = 0; i < 7; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }


    // Construimos la url

    String url = URL_P_M + MOTION_ENDPOINT;
    
    // Mandamos la petición

    http.begin(client, url.c_str());
    http.addHeader("Authorization", "Bearer " + token);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST("{}");

    // Si la respuesta es correcta parpadeamos los leds

    if(!(code == 200 || code == 201)){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
    }
  }
}

// Construye un json con la humedad y temperaturas actuales

String getTemperatureAndHumidityJson(float humidity, float celsius, float fahrenheit)
{
  String result = "{";

  result += "\"humidity\":\"";
  result.concat(humidity);
  result += "\",";

  result += "\"celcius\":\"";
  result.concat(celsius);
  result += "\",";

  result += "\"fahrenheit\":\"";
  result.concat(fahrenheit);
  result += "\",";

  result += "\"heatIndexc\":\"";
  result.concat(dht.computeHeatIndex(fahrenheit, humidity));
  result += "\",";

  result += "\"heatIndexf\":\"";
  result.concat(dht.computeHeatIndex(celsius, humidity, false));
  result += "\"";

  result += "}";

  return result;
}

// Obtiene los datos del ambiente

void getTemperatureAndHumiditySensor()
{
  // Leemos los valores

  float humidity = dht.readHumidity();
  float celsius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);

  // Comprobamos las medidas

  if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit))
  {
    // En caso de error indicamos que el sensor está desconectado

    s.send(409, "application/json", "{\"error\":\"Sensor desconectado\"}");
  }
  else
  {
    // Si no retornamos los valores
    s.send(200, "application/json", getTemperatureAndHumidityJson(humidity, celsius, fahrenheit));
  }
}

// Inicializa los websockets
void InitWebSockets()
{
  webSocket.begin();
  webSocketMotion.begin();
}

// Envía los valores de los sensores por websocket
void broadcastSensors()
{
  // Obtenemos los valores por websocket

  float humidity = dht.readHumidity();
  float celsius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit))
  {
    // Si hay un error en el sensor indicamos que el sensor está desconectado

    webSocket.broadcastTXT("{\"error\":\"Sensor desconectado\"}");
  }
  else
  {
    // Si no enviamos los datos
    webSocket.broadcastTXT(getTemperatureAndHumidityJson(humidity, celsius, fahrenheit).c_str());
  }
}

// Actualiza los datos del sensor y envía la petición

void updateSensor() {

  // Obtenemos los valores

  float humidity = dht.readHumidity();
  float celsius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);

  // Si hay error hacemos un parpadeo lento
  if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit))
  {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
  }
  else {

    // Si todo está correcto construimos la url

    String url = URL_P_M + TEMP_HUMIDITY_ENDPOINT;
    

    // Enviamos los datos

    http.begin(client, url.c_str());
    http.addHeader("Authorization", "Bearer " + token);
    http.addHeader("Content-Type", "application/json");
    int responseCode = http.POST(getTemperatureAndHumidityJson(humidity, celsius, fahrenheit).c_str());


    Serial.println(responseCode);

    // Si no es correcto el código  realizamos un parpadeo lento

    if (responseCode != 200) {

        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
    }
  }
}

// Checkea si hay un movimiento y lo envía por websocket
void checkMotionWebSocket()
{
  if (checkMotion())
  {
    webSocketMotion.broadcastTXT("");
  }
}