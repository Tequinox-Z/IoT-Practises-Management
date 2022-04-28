#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <string>

HTTPClient http; 
WiFiClient client;

void scanWifi()
{ 
  int networksFound =  WiFi.scanNetworks();
  String jsonAPs = "{\"networks\": [";

  for (int i = 0; i < networksFound; i++)
  {
    jsonAPs = jsonAPs + "\"" + dBmtoPercentage(WiFi.RSSI(i)) + "\":\"" + WiFi.SSID(i) + "\",";
  }

  if (networksFound != 0) {
    jsonAPs.remove(jsonAPs.length() - 1, 1);
  }

  jsonAPs += "]}";

  s.send(200, "application/json", jsonAPs);
}

void checkPIRsensor() {
  int value= digitalRead(PIR_PIN);
  
  if (value == HIGH) {

    for (int i = 0; i < 7; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW); 
        delay(100);
    }
  
    String se = "http://192.168.1.134:8080/school";
  
    http.begin(client, se.c_str());
    http.addHeader("Authorization", "Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJVc2VyIERldGFpbHMiLCJpc3MiOiJQcmFjdGlzZXMvTWFuYWdlbWVudCIsImV4cCI6MTcwODMwOTgyMCwiaWF0IjoxNjQ4MzA5ODIwLCJ1c2VybmFtZSI6IjQ4MTI0NTM4TSJ9.5T-lJt2r4mDFmu0r-ibs_ydwM96F94Zfu9Bk8RHN308");
    http.addHeader("Content-Type", "application/json");
    http.POST("{}");

  }
}
