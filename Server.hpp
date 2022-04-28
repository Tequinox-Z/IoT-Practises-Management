

// new

void InitServer()
{
  s.on("/network", HTTP_GET, scanWifi);
  s.serveStatic("/", SPIFFS, "/");
  s.begin();
}
