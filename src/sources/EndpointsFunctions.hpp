#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <string>
#include "user_interface.h"

void configure()
{
  String response = s.arg("plain").c_str();
  Serial.println("Response: " + response);

  if (response.length() > 1024) {
    s.send(400, "application/json",  "{\"status\":\"Tamaño máximo del archivo excedido (1024 caracteres máximo)\"}");
  }

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, response);

  String ssid = doc["ssid"];
  String password = doc["password"];
  String user = doc["user"];
  String userPsk = doc["passwordUser"];

  if (ssid.length() == 0 || user.length() == 0 || userPsk.length() == 0)
  {
    s.send(400, "application/json", "{\"status\":\"El SSID no puede estar vacío\"}");
  }
  else {
    s.send(201, "application/json");
    
    File configFile = SPIFFS.open("/config.json", "w");
    configFile.print(response.c_str());

    configFile.close();
        
    ESP.reset();
  }
  }

void scanWifi()
{
  int networksFound = WiFi.scanNetworks();
  String jsonAPs = "{\"networks\": [";

  for (int i = 0; i < networksFound; i++)
  {
    jsonAPs = jsonAPs + "{\"signal\":\"" + dBmtoPercentage(WiFi.RSSI(i)) + "\",";
    jsonAPs = jsonAPs + "\"ssid\":\"" + WiFi.SSID(i) + "\",";
    jsonAPs = jsonAPs + "\"encryptionType\":\"" + convertIntToTypeEncrypt(WiFi.encryptionType(i)) + "\"";
    jsonAPs = jsonAPs + "},";
  }

  if (networksFound != 0)
  {
    jsonAPs.remove(jsonAPs.length() - 1, 1);
  }

  jsonAPs += "]}";

  s.send(200, "application/json", jsonAPs);
}

boolean checkMotion()
{
  return digitalRead(PIR_PIN) == HIGH;
}

void checkPIRsensor()
{
  if (checkMotion())
  {
    for (int i = 0; i < 7; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }

    String url = URL_P_M + MOTION_ENDPOINT;
    
    http.begin(client, url.c_str());
    http.addHeader("Authorization", "Bearer " + token);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST("{}");

    if(!(code == 200 || code == 201)){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
    }
  }
}

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

void getTemperatureAndHumiditySensor()
{
  float humidity = dht.readHumidity();
  float celsius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit))
  {
    s.send(409, "application/json", "{\"error\":\"Sensor desconectado\"}");
  }
  else
  {
    s.send(200, "application/json", getTemperatureAndHumidityJson(humidity, celsius, fahrenheit));
  }
}

void InitWebSockets()
{
  webSocket.begin();
  webSocketMotion.begin();
}

void broadcastSensors()
{
  float humidity = dht.readHumidity();
  float celsius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit))
  {
    webSocket.broadcastTXT("{\"error\":\"Sensor desconectado\"}");
  }
  else
  {
    webSocket.broadcastTXT(getTemperatureAndHumidityJson(humidity, celsius, fahrenheit).c_str());
  }
}

void updateSensor() {
  float humidity = dht.readHumidity();
  float celsius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit))
  {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
  }
  else {
    String url = URL_P_M + TEMP_HUMIDITY_ENDPOINT;
    
    http.begin(client, url.c_str());
    http.addHeader("Authorization", "Bearer " + token);
    http.addHeader("Content-Type", "application/json");
    int responseCode = http.POST(getTemperatureAndHumidityJson(humidity, celsius, fahrenheit).c_str());

    Serial.println(responseCode);

    if (responseCode != 200) {

        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
    }
  }
}

void checkMotionWebSocket()
{
  if (checkMotion())
  {
    webSocketMotion.broadcastTXT("");
  }
}