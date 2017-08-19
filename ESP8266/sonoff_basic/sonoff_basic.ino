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
  webPage = "<h1>SONOFF Web Server 2</h1><div style=\"height:80px;width:80px;background-color:";
  webPage += (mStatus == 0? "red": "green") ;
  webPage += ";\"/><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>"; 
}

void setup(void){
 generatePage(); 
  
  // preparing GPIOs
  pinMode(gpio13Led, OUTPUT);
  digitalWrite(gpio13Led, HIGH);
  
  pinMode(gpio12Relay, OUTPUT);
  digitalWrite(gpio12Relay, LOW); //relay is OPEN by default!
 
  Serial.begin(115200); 
  delay(5000);
  WiFi.begin(ssid, password);
  Serial.println("");

uint8_t dummy = 0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED && dummy < 10) {
    
  digitalWrite(gpio13Led, LOW);
  
    //https://github.com/esp8266/Arduino/issues/2186
    WiFi.persistent(false);
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    delay(500);
  digitalWrite(gpio13Led, HIGH);
    delay(500);
    Serial.print(".");
    dummy++;
  }

  if (dummy == 10){
    Serial.println("");
    Serial.println("[Ok, assuming that we are connected...]");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
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
    delay(1000);
  });
  server.on("/off", [](){
    digitalWrite(gpio13Led, HIGH);
    digitalWrite(gpio12Relay, LOW);
    mStatus = 0;
    generatePage();

    server.send(200, "text/html", webPage);
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 

