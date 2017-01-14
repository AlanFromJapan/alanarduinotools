/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "Kalshagar-wifi-g";
const char* password = "tamisevil666";

ESP8266WebServer server(80);

String webPage = "";

int gpio13Led = 13;
int gpio12Relay = 12;

void setup(void){
  webPage += "<h1>SONOFF Web Server</h1><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>";  
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
    server.send(200, "text/html", webPage);
  });
  server.on("/on", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio13Led, LOW);
    digitalWrite(gpio12Relay, HIGH);
    delay(1000);
  });
  server.on("/off", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio13Led, HIGH);
    digitalWrite(gpio12Relay, LOW);
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 

