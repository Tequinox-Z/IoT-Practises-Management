

// new

void InitServer()
{
  s.on("/network", HTTP_GET, scanWifi);
  s.on("/sensor", HTTP_GET, getTemperatureAndHumiditySensor);
  s.serveStatic("/", SPIFFS, "/");
  s.begin();
}
