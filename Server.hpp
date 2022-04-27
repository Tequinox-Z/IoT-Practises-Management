void InitServer()
{

  // Endpoint en modo estación
  
    server.on("/sensors", HTTP_GET, getInfoSensors).setFilter(ON_STA_FILTER);

  // End-points en modo AP
  
    server.on("/", HTTP_GET, getIndex).setFilter(ON_AP_FILTER);
    server.on("/scan", HTTP_POST, scanSta).setFilter(ON_AP_FILTER);
    server.on("/getAps", HTTP_GET, getAps).setFilter(ON_AP_FILTER);
    server.on("/configuration", HTTP_POST, setConfiguration).setFilter(ON_AP_FILTER);

  // End-point para ficheros estáticos
  
    server.serveStatic("/file/", SPIFFS, "/file/");

  // End-point para rutas no encontradas
  
    server.onNotFound([](AsyncWebServerRequest *request){
      request->send(404);
    });

  // Iniciamos el servidor
    
    server.begin();
}
