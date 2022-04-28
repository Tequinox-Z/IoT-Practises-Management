#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include "Config.hpp"

ESP8266WebServer s(80);
DHT dht(TEMPERATURE_PIN, DHT11);

#include "Utils.hpp"
#include "EndpointsFunctions.hpp"
#include "Server.hpp"

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
