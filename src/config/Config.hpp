/* SSID del AP de configuración */

  char* SSID_AP = "Practises Management";

/* Conexión vía Hostname */

  const char* hostname = "Node-School";

/* Pin del sensor de Temperatura y humedad */

  const int TEMPERATURE_PIN = 12;
  
/* Pin del sensor PIR */

  const int PIR_PIN = 5;

/* Pin RESET (Opcional)  */

  const int RESET_PIN = 4;

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

/* Tiempo de espera de conexión Wi-Fi  */

  const long TIME_WAIT_WIFI = 10000;

/* Tiempo de actualización de temperatura y humedad */

  const int SENSOR_UPDATE = 10000;

/* URL */

  // const String URL_P_M = "http://192.168.1.134:8080/";
  
  const String URL_P_M = "https://practisesmanagement2022.herokuapp.com/";
  const String LOGIN = "auth/login-iot";
  const String MOTION_ENDPOINT = "motion";
  const String TEMP_HUMIDITY_ENDPOINT = "temp-humidity";