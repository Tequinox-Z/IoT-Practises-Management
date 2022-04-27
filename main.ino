
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <DNSServer.h>

AsyncWebServer server(80);

#include "config.h"
#include "Utilidades.hpp"
#include "functionsEndpoints.hpp"
#include "Server.hpp"

boolean getCredentials = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.flush();
  delay(1000);
  Serial.println("Bienvenido a Practises Management IoT");
  Serial.flush();

  Serial.println("Iniciando dispositivo... ");

  if (getCredentials) {
    ConnectWiFi_STA("test", "test");
  }
  else {
    if (!SPIFFS.begin()) {
      Serial.println("No se ha podido montar SPIFFS");
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
}
