#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>//thư viện giao tiếp 
// Replace with your network credentials
#include <FirebaseArduino.h>
#include "DHT.h" 

const char* ssid = "Simon Phu";
const char* password = "Qu@ngphu92";
#define FIREBASE_HOST "smartpro-a82e8.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "zdcJHBdXwSjMcWyX4QEGDFzOCWVLNgib49bj9ce4"
String pushstatus = "";
String pullstatus = "";
String ledstatus = "";
// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
          
const int DHTPIN = 4;      
const int DHTTYPE = DHT11;  
const int d7 = 13;
const int d0 = 16;
const int d6 = 12;

 
DHT dht(DHTPIN, DHTTYPE);
// Current time

void setup() {

  Serial.begin(115200);
  pinMode(d7, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d0, OUTPUT);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  server.begin();
  dht.begin();       
}

void loop() {
      float t = dht.readTemperature(); 
      Serial.print("Nhiet do: ");
      Serial.println(t);               
      pushstatus = Firebase.getString("vuonrau/push/status");                                    // get ld status input from firebase
      pullstatus = Firebase.getString("vuonrau/pull/status");  
      ledstatus = Firebase.getString("vuonrau/led/status");  
      Firebase.setString("vuonrau/tmp", String(t));    
      if(t > 31 ){
        Firebase.setString("vuonrau/push/status", "on");
        Firebase.setString("vuonrau/led/status", "off");
        Firebase.setString("vuonrau/pull/status", "off");
      }else if(t >= 28 && t <= 31){
        Firebase.setString("vuonrau/pull/status", "on");
        Firebase.setString("vuonrau/push/status", "off");
        Firebase.setString("vuonrau/led/status", "off");
      }else{
        Firebase.setString("vuonrau/pull/status", "off");
        Firebase.setString("vuonrau/push/status", "on");
         Firebase.setString("vuonrau/led/status", "on");
      }
//      ==================PUSH================================
      if (pushstatus == "on") {                                                        
        digitalWrite(LED_BUILTIN, LOW);                                                
        digitalWrite(d7, HIGH);                                                        
      }
      else if (pushstatus == "off") {                                                  
        digitalWrite(LED_BUILTIN, HIGH);                                               
        digitalWrite(d7, LOW);                                                         
      }
      else {
        Serial.println("Wrong Credential! Please send on/off");
      }
//      ==================PULL================================
      if (pullstatus == "on") {                                                        
        digitalWrite(LED_BUILTIN, LOW);                                                
        digitalWrite(d6, HIGH);                                                        
      }
      else if (pullstatus == "off") {                                                  
        digitalWrite(LED_BUILTIN, HIGH);                                               
        digitalWrite(d6, LOW);                                                         
      }
      else {
        Serial.println("Wrong Credential! Please send on/off");
      }
//      ==================LED================================ 
      if (ledstatus == "on") {                                                        
        digitalWrite(LED_BUILTIN, LOW);                                                
        digitalWrite(d0, HIGH);                                                        
      }
      else if (ledstatus == "off") {                                                  
        digitalWrite(LED_BUILTIN, HIGH);                                               
        digitalWrite(d0, LOW);                                                         
      }
      else {
        Serial.println("Wrong Credential! Please send on/off");
      }
}
