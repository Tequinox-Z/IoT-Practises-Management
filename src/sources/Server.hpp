


// new

void InitServer()
{
  s.on("/network", HTTP_GET, scanWifi);
  s.on("/sensor", HTTP_GET, getTemperatureAndHumiditySensor);
  s.on("/connectWifi", HTTP_POST, connectWifi);
  s.serveStatic("/", SPIFFS, "/");
  s.begin();
}
