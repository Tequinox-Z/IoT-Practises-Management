#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include "config/Config.hpp"
#include <WebSocketsServer.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

HTTPClient http;
WiFiClient client;

ESP8266WebServer s(80);
WebSocketsServer webSocket = WebSocketsServer(81);
WebSocketsServer webSocketMotion = WebSocketsServer(82);

DHT dht(TEMPERATURE_PIN, DHT11);
DynamicJsonDocument configuration(1024);
long timeMillis;

#include "sources/Utils.hpp"
#include "sources/EndpointsFunctions.hpp"
#include "sources/Server.hpp"

boolean getCredentials;

void setup()
{
  Serial.begin(115200);
  digitalWrite(LED_BUILTIN, HIGH);
  dht.begin();

  if (!SPIFFS.begin())
  {
    ESP.reset();
  }

  if (SPIFFS.exists("/config.json"))
  {
    getCredentials = false;

    readConfig();
    connectWiFi();

    if (configuration["token"] == nullptr)
    {
      setToken();
      readConfig();
    }
  }
  else
  {
    getCredentials = true;

    InitWebSockets();
    setupAP();
  }

  // up
  InitServer();
}

void loop()
{
  if (!getCredentials)
  {
    if ((millis() - timeMillis) > SENSOR_UPDATE) {
      timeMillis = millis();
      updateSensor();
    }
    checkPIRsensor();
  }
  else
  {
    loopAP();
    webSocket.loop();
    webSocketMotion.loop();

    broadcastSensors();
    checkMotionWebSocket();
  }

  // up
  s.handleClient();
}