/* SSID del AP de configuración */

  char* SSID_AP = "Practises Management";

/* Conexión vía Hostname */

  const char* hostname = "Node-School";

/* Pin de Temperatura y humedad */

  const int TEMPERATURE_PIN = A0;

/* Pin del sensor PIR */

  const int PIR_PIN = 5;

/* Dirección IP, puerta de enlace, máscara de red */

  IPAddress ip(192, 168, 1, 200);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

/* Dirección de red en el modo AP */

  IPAddress apIP(192, 168, 1, 1);

 /* Puerto DNS */
 
  const byte DNS_PORT = 53;

/* Indicadores de recibida de señan Wi-Fi de los puntos de acceso del alrededor*/

  const int RSSI_MAX =-50;
  const int RSSI_MIN =-100;
