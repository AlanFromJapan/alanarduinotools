/*********
 *  AlanFromJapan (http://electrogeek.cc/sonoff.html)
 *  Based on the work of (and thanks for sharing!)
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "PrivateConstants.h"

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = WIFI_NAME;
const char* password = WIFI_PASSWORD;

ESP8266WebServer server(80);

String webPage = "";

int gpio13Led = 13;
int gpio12Relay = 12;
volatile byte mStatus = 0;

void generatePage(){
  webPage = "<h1>SONOFF Web Server 2</h1><div style=\"height:300px;width:300px;background-color:";
  webPage += (mStatus == 0? "red": "green") ;
  webPage += ";\"/><p><a href=\"on\"><button style=\"width:100px;\">ON</button></a>&nbsp;<a href=\"off\"><button style=\"width:100px;\">OFF</button></a></p>"; 
}

void setup(void){
 generatePage(); 
  
  // preparing GPIOs
  pinMode(gpio13Led, OUTPUT);
  digitalWrite(gpio13Led, HIGH);
  
  pinMode(gpio12Relay, OUTPUT);
  digitalWrite(gpio12Relay, LOW); //relay is OPEN by default!
 
  Serial.begin(115200); 
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection (blinking)
  while (WiFi.status() != WL_CONNECTED) {    
    digitalWrite(gpio13Led, LOW);
    delay(500);
    digitalWrite(gpio13Led, HIGH);
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("sonoff2", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    generatePage();
    server.send(200, "text/html", webPage);
  });
  server.on("/on", [](){
    digitalWrite(gpio13Led, LOW);
    digitalWrite(gpio12Relay, HIGH);
    mStatus = 1;
    generatePage();

    server.send(200, "text/html", webPage);
  });
  server.on("/off", [](){
    digitalWrite(gpio13Led, HIGH);
    digitalWrite(gpio12Relay, LOW);
    mStatus = 0;
    generatePage();

    server.send(200, "text/html", webPage);
  });

/*
  server.on("/favicon.ico", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });
*/

  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
  mdns.update();
} 

