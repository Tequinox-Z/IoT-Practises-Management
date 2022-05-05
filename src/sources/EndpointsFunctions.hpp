#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <string>
#include "user_interface.h"

HTTPClient http;
WiFiClient client;

void connectWifi()
{
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, s.arg("plain").c_str());

  String ssid = doc["ssid"];
  String password = doc["password"];

  if (ssid.length() == 0)
  {
    s.send(400, "application/json", "{\"status\":\"El SSID no puede estar vacío\"}");
  }
  else
  {
    if (password.length() == 0)
    {
      WiFi.begin(ssid);
    }
    else
    {
      WiFi.begin(ssid, password);
    }

    delay(4000);

    if (WiFi.status() != WL_CONNECTED)
    {
      String status;

      switch (wifi_station_get_connect_status())
      {
      case 1:
        status = "Conectando";
        break;
      case 2:
        status = "Contraseña incorrecta";
        break;
      case 3:
        status = "AP no encontrado";
        break;
      case 4:
        status = "Fallo al conectar";
        break;
      case 5:
        status = "Error al obtener dirección IP";
        break;
      }

      s.send(409, "application/json", "{\"status\":\"" + status + "\"}");
    }
    else
    {
      s.send(201, "application/json", "{}");
    }
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

    String se = "http://192.168.1.134:8080/school";

    http.begin(client, se.c_str());
    http.addHeader("Authorization", "Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJVc2VyIERldGFpbHMiLCJpc3MiOiJQcmFjdGlzZXMvTWFuYWdlbWVudCIsImV4cCI6MTcwODMwOTgyMCwiaWF0IjoxNjQ4MzA5ODIwLCJ1c2VybmFtZSI6IjQ4MTI0NTM4TSJ9.5T-lJt2r4mDFmu0r-ibs_ydwM96F94Zfu9Bk8RHN308");
    http.addHeader("Content-Type", "application/json");
    http.POST("{}");
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

void checkMotionWebSocket()
{
  if (checkMotion())
  {
    webSocketMotion.broadcastTXT("");
  }
}