AsyncWebServer serverAP(80);
DNSServer dnsServerAP;

void setupAP(char* ssid = SSID_AP) {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid);

  dnsServerAP.start(DNS_PORT, "*", apIP);
}

void loopAP() {
  dnsServerAP.processNextRequest();
}

void ConnectWiFi_STA(bool useStaticIP = false, char* ssid = "", char* password = "")
{
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   
   if(useStaticIP) WiFi.config(ip, gateway, subnet);
   while (WiFi.status() != WL_CONNECTED) 
   { 
     
   }
     Serial.print(F("IP Address: "));
     Serial.println(WiFi.localIP());
}


int dBmtoPercentage(int dBm)
{
  int quality;
  if (dBm <= RSSI_MIN)
  {
    quality = 0;
  }
  else if (dBm >= RSSI_MAX)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (dBm + 100);
  }

  return quality;
}
