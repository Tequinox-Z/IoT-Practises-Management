
// Inicializa los end-poitns y asigna las funciones
// correspondientes

void InitServer()
{
  
  s.on("/network", HTTP_GET, scanWifi);                                       // Obtiene las redes Wi-Fi
  s.on("/sensor", HTTP_GET, getTemperatureAndHumiditySensor);                 // Obtiene el valor de los sensores
  s.on("/configure", HTTP_POST, configure);                                   // Configura el dispositivo
  s.serveStatic("/", SPIFFS, "/");                                            // Sirve ficheros
  s.begin();                                                                  // Inicializa el servidor
}
