#include <ESP8266WiFi.h>
//#include <ESPAsyncWebServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <DNSServer.h>

//AsyncWebServer server(80);
ESP8266WebServer s(80);

#include "Config.hpp"
#include "Utils.hpp"
#include "EndpointsFunctions.hpp"
#include "Server.hpp"

boolean getCredentials = false;

void setup() {
  digitalWrite(LED_BUILTIN, HIGH); 
  
  if (getCredentials) {
    //ConnectWiFi_STA("test", "test");
  }
  else {
    if (!SPIFFS.begin()) {
      ESP.restart();
    }

    setupAP();
  }

  InitServer();
}

void loop() {
  if (getCredentials) {
    checkPIRsensor();
    delay(2000);
  }
  else {
    loopAP();
  }
  s.handleClient();
}
