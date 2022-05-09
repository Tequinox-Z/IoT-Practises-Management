#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <string>
#include "user_interface.h"

void configure()
{
  String response = s.arg("plain").c_str();
  Serial.println("Response: " + response);

  if (response.length() > 1024) {
    s.send(400, "application/json",  "{\"status\":\"Tamaño máximo del archivo excedido (1024 caracteres)\"}");
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
    File configFile = SPIFFS.open("/config.json", "w");
    configFile.print(response.c_str());

    configFile.close();
        
    s.send(201, "application/json");

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
    String token = configuration["token"];
    
    http.begin(client, url.c_str());
    http.addHeader("Authorization", "Bearer " + token);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST("{}");
    
    
    if (code == 400) {
      DynamicJsonDocument error(1024);
      deserializeJson(error, http.getString());
      String errorMessage = error["message"];
      Serial.println(errorMessage);
      generateError(errorMessage);
      ESP.reset();
    }
    else if(!(code == 200 || code == 201)){
      Serial.println("Servidor no disponible");
    }
  }
}

String getTemperatureAndHumidityJson(float humidity, float celsius, float fahrenheit)
{
  String result = "{";

  result += "\"humidity\":\"";
  result.concat(humidity);
  result += "\",";

  result += "\"tempCelcius\":\"";
  result.concat(celsius);
  result += "\",";

  result += "\"tempFahrenheit\":\"";
  result.concat(fahrenheit);
  result += "\",";

  result += "\"heatIndexFahrenheit\":\"";
  result.concat(dht.computeHeatIndex(fahrenheit, humidity));
  result += "\",";

  result += "\"heatIndexCelsius\":\"";
  result.concat(dht.computeHeatIndex(celsius, humidity, false));
  result += "\",";

  result += "\"pin\":\"";
  result.concat(TEMPERATURE_PIN);
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
    Serial.println("Sensor desconectado");
  }
  else {
    String url = URL_P_M + TEMP_HUMIDITY_ENDPOINT;
    String token = configuration["token"];
    
    http.begin(client, url.c_str());
    http.addHeader("Authorization", "Bearer " + token);
    http.addHeader("Content-Type", "application/json");
    http.PUT(getTemperatureAndHumidityJson(humidity, celsius, fahrenheit).c_str());
  }
}

void checkMotionWebSocket()
{
  if (checkMotion())
  {
    webSocketMotion.broadcastTXT("");
  }
}