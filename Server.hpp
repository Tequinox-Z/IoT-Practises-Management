

// new

void InitServer()
{
  s.on("/network", HTTP_GET, scanWifi);
  s.on("/sensor-temp", HTTP_GET, getTemperatureAndHumiditySensor);
  s.serveStatic("/", SPIFFS, "/");
  s.begin();
}
