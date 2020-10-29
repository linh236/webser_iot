#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "EEPROM.h"
#define  BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <BlynkSimpleEsp8266.h>
   
 const char* ssid   = "FifthBit";  
 const char* password = "ThisIsNotMyRealPassword";  
 const char* restURL = "http://192.168.1.100:3000/measures";  
 int maxWiFiConnectionWait = 20; // Seconds  
   
 bool connectWiFi();  
 bool sensorValues(float&, float&);  
 String toJSON(float, float);  
 int postValues(String json);  
   
 DHT_Unified dht(DHTPIN, DHTTYPE);  
   
 void setup() {  
  Serial.begin(115200);  
  dht.begin();  
 }  
   
 void loop() {  
   
  Serial.println("Wait 5.");  
  delay(5000);  
   
  if (!connectWiFi()) {  
   Serial.println("No WiFi");  
   return;  
  }  
  Serial.println("WiFi OK");  
   
  float temperature;  
  float humidity;  
  if (!sensorValues(temperature, humidity)) {  
   Serial.println("No sensor values");  
   return;  
  }  
   
  String json = toJSON(temperature, humidity);  
  int httpCode = postValues(json);  
    
  if (httpCode == 201) {  
   Serial.print("POSTED: ");  
   Serial.println(json);  
   delay(55000);  
  } else {  
   Serial.print("Fail. HTTP ");  
   Serial.println(httpCode);  
  }  
   
 }  
   
 int postValues(String json) {  
    
  HTTPClient http;   //Declare object of class HTTPClient    
  http.begin(restURL); //Specify request destination  
  http.addHeader("Content-Type", "application/json"); //Specify content-type header  
  http.addHeader("Accept", "application/json");  
  int httpCode = http.POST(json);  //Send the request  
  String payload = http.getString();         //Get the response payload  
   
  return httpCode;  
   
 // Serial.print("HTTP Response: ");  //Print HTTP return code  
 // Serial.print(httpCode);  
 // Serial.println(payload);  //Print request response payload  
 }  
   
 String toJSON(float temperature, float humidity) {  
   
  String temp = String(temperature);  
  String humi = String(humidity);  
  return String("{\"temperature\": ") + temp + ", \"humidity\": " + humi + "}";  
 }  
   
 bool sensorValues(float &temp, float &humid) {  
   
  sensors_event_t event;  
   
  dht.temperature().getEvent(&event);   
  if (isnan(event.temperature)) {  
   return false;  
  } else {  
   temp = event.temperature;  
  }  
   
  dht.humidity().getEvent(&event);  
  if (isnan(event.relative_humidity)) {  
   return false;  
  } else {  
   humid = event.relative_humidity;  
  }  
   
  return true;  
 }  
   
 void connectSensor() {  
   
  Serial.println("Starting sensor");  
 }  
   
 bool connectWiFi() {  
   
  if (WiFi.status() == WL_CONNECTED) return true;  
   
  int secondsWaited = 0;  
  Serial.print("Connecting to ");  
  Serial.println(ssid);  
   
  delay(1000);  
  WiFi.begin(ssid, password);  
    
  while (WiFi.status() != WL_CONNECTED) {  
   if (secondsWaited > maxWiFiConnectionWait) {  
    Serial.println("FAILED");  
    return false;  
   }  
   secondsWaited++;  
   delay(1000);  
   Serial.print(".");  
  }  
    
  Serial.println();  
  Serial.print("Connected: ");  
  Serial.println(WiFi.localIP());  
  return true;  
 }  
