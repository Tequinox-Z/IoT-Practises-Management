#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include "config/Config.hpp"
#include <WebSocketsServer.h>

ESP8266WebServer s(80);
WebSocketsServer webSocket = WebSocketsServer(81);
WebSocketsServer webSocketMotion = WebSocketsServer(82);

DHT dht(TEMPERATURE_PIN, DHT11);

#include "sources/Utils.hpp"
#include "sources/EndpointsFunctions.hpp"
#include "sources/Server.hpp"

boolean getCredentials = false;
  
void setup() {
  digitalWrite(LED_BUILTIN, HIGH); 
  dht.begin();

  if (getCredentials) {
    //ConnectWiFi_STA("test", "test");
  }
  else {
    if (!SPIFFS.begin()) {
      ESP.restart();
    }

    InitWebSockets();
    setupAP();
  }

  InitServer();
}

void loop() {
  if (getCredentials) {
    checkPIRsensor();
    delay(1000);
  }
  else {
    loopAP();
    webSocket.loop();
    webSocketMotion.loop();
    broadcastSensors();
    checkMotionWebSocket();
    delay(100);
  }
  s.handleClient();
}