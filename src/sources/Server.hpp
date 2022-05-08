void InitServer()
{
  s.on("/network", HTTP_GET, scanWifi);
  s.on("/sensor", HTTP_GET, getTemperatureAndHumiditySensor);
  s.on("/configure", HTTP_POST, configure);
  s.serveStatic("/", SPIFFS, "/");
  s.begin();
}
