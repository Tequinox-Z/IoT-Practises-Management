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

String convertIntToTypeEncrypt(int wifiEncryp) {
  String encrypt = "";

  switch (wifiEncryp) {
    case ENC_TYPE_WEP:
      encrypt = "WEP";
      break;
    case ENC_TYPE_TKIP:
      encrypt = "WPA/PSK";
      break;
    case ENC_TYPE_CCMP:
      encrypt = "WPA2/PSK";
      break;
    case ENC_TYPE_NONE:
      encrypt = "open";
      break;
    case ENC_TYPE_AUTO:
      encrypt = "WPA/WPA2/PSK";
      break;
  }
  return encrypt;
}

void ConnectWiFi_STA(bool useStaticIP = false, char* ssid = "", char* password = "")
{
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   
   if(useStaticIP) WiFi.config(ip, gateway, subnet);
   while (WiFi.status() != WL_CONNECTED) 
   { 
     
   }
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

