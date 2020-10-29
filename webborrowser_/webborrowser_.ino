#include <ESP8266WiFi.h>                                                // esp8266 library
#include <FirebaseArduino.h>                                             // firebase library
//#include <ArduinoJson.h>                                             // firebase library

#define FIREBASE_HOST "https://iotpro-58c44.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "F4mMmNXp1CPYvJYX5KwtrLifqw6UvVO4fyCUKhoj"                    // the secret key generated from firebase
const char* ssid     = "NEWS";
const char* password = "1651120032";

String fireStatus = "";                                                     // led status received from firebase
int led = 15;                                                                   // for external led
void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(led, OUTPUT);                 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());                                                 //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
  Firebase.setString("LED_STATUS", "OFF");                                          //send initial string of led status
}

void loop() {
  fireStatus = Firebase.getString("led1/LED_STATUS");                                      // get ld status input from firebase
  String led1 = Firebase.getString("led1/id");  
  if (fireStatus == "ON") {                                                          // compare the input of led status received from firebase
    Serial.println("Led Turned ON");
    Serial.println(led1);                         
    digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
    digitalWrite(led, HIGH);                                                         // make external led ON
  } 
  else if (fireStatus == "OFF") {                                                  // compare the input of led status received from firebase
    Serial.println("Led Turned OFF");
    digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
    digitalWrite(led, LOW);                                                         // make external led OFF
  }
  else {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }
}
