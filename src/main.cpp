
//  Importamos las librerias

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include "config/Config.hpp"
#include <WebSocketsServer.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


// ==========================================================                                           ==========================================================


                                                              // Dispositivo IoT de Practises Management
                                                                       // Salvador Pérez - 2022

// ==========================================================                                           ==========================================================

// Inicializamos los recursos necesarios

HTTPClient http;
WiFiClientSecure client;
String token;

// Iniciamos el servidor y los websockets

ESP8266WebServer s(80);
WebSocketsServer webSocket = WebSocketsServer(81);
WebSocketsServer webSocketMotion = WebSocketsServer(82);

// Configuramos el sensor de temperatura

DHT dht(TEMPERATURE_PIN, DHT11);
DynamicJsonDocument configuration(1024);
long timeMillis;

#include "sources/Utils.hpp"
#include "sources/EndpointsFunctions.hpp"
#include "sources/Server.hpp"

boolean getCredentials;                     // Esta variable indicará si existen o no credenciales

void setup()
{
  Serial.begin(115200);                     // Iniciamos una consola serial con un bitrate de 115200 bits/s
  dht.begin();                              // Iniciamos el sensor de temperatura
 
  pinMode(RESET_PIN, INPUT);                // Establecemos el pin de reseteo
  pinMode(LED_BUILTIN, OUTPUT);             //                                y el de la luz de indicación

  // Iniciamos el almacenamiento

  if (!SPIFFS.begin())
  {
    ESP.reset();                            // Si no se puede iniciar el almacenamiento reseteamos el dispositivo
  }

  if (SPIFFS.exists("/config.json"))        // Comprobamos si existen credenciales
  {
    getCredentials = false;                 // Si existe le indicamos que no hay que pedir credenciales

    readConfig();                           // Cargamos la configuración 
    connectWiFi();                          // Iniciamos el ap
    setToken();                             // Pedimos al servidor el token
  }
  else
  {
    getCredentials = true;                  // Si no hay credenciales

    InitWebSockets();                       // Iniciamos los websockets
    setupAP();                              // Arrancamos el AP
    InitServer();                           // Iniciamos el servidor rest y de ficheros
  }

  
}

void loop()
{
  if (!getCredentials)                                // Si no hay que pedir credenciales actualizamos la información de los sensores
  {
    if ((millis() - timeMillis) > SENSOR_UPDATE)      // Miramos si toca refrescar el sensor
    {
      timeMillis = millis();
      updateSensor();                                 // Actualizamos los datos del sensor
    }
    checkPIRsensor();                                 // Comprobamos si hay movimientos

    byte value = digitalRead(RESET_PIN);              // Comprobamos si el usuario ha pulsado el botón de reset

    if (value == HIGH)
    {
      digitalWrite(LED_BUILTIN, LOW);                 // Si el botón ha sido pulsado apagamos el led
      resetConfig();                                  // y reseteamos la configuración
    }

    digitalWrite(LED_BUILTIN, HIGH);                  // Realizamos un aviso visual de reseteo
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  else
  {
    loopAP();                                         // En caso de no tener credenciales comprobamos y conectamos si hay usuarios que se quieran conectar al ap
    webSocket.loop();                                 // Procesamos las peticiones por los websockets
    webSocketMotion.loop();

    broadcastSensors();                               // Emitimos los valores del sensor de temperatura
    checkMotionWebSocket();                           // Si hay movimiento mandamos un mensaje
    s.handleClient();                                 // Procesamos las respuestas de los clientes hacia el servidor 
  }
}