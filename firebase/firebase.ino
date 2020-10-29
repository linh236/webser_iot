/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>  
#include <Wire.h>
#include "RTClib.h"
#include <Timezone.h>

RTC_DS3231 rtc;
//DS3232RTC rtc;
// wifi
const char* ssid     = "NEWS";
const char* password = "1651120032";
//const char* host = "testiot-d9f74.firebaseio.com/";          
//const char* auth = "4Ku3ygUrZcLyXfj6QL0EpbNxB0q1RUiZFRvwPYwr" ;   
#define FIREBASE_HOST "iotpro-58c44.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "F4mMmNXp1CPYvJYX5KwtrLifqw6UvVO4fyCUKhoj"   

// Auxiliar variables to store the current output state
String output5State = "off";
// Assign output variables to GPIO pins
const int d0 = 16;
const int d1 = 5;
const int d2 = 4;
const int d3 = 0;
const int d5 = 14;
const int d6 = 12;
const int d7 = 13;
const int d8 = 15;

String fireStatus0 = "";
String turnon0 = "";
String turnoff0 = "";
String fireStatus1 = "";
String turnon1 = "";
String turnoff1 = "";
String fireStatus2 = "";
String turnon2 = "";
String turnoff2 = "";
String fireStatus3 = "";
String turnon3 = "";
String turnoff3 = "";
String fireStatus5 = "";
String turnon5 = "";
String turnoff5 = "";
String fireStatus6 = "";
String turnon6 = "";
String turnoff6 = "";
String fireStatus7 = "";
String turnon7 = "";
String turnoff7 = "";
String fireStatus8 = "";
String turnon8 = "";
String turnoff8 = "";
String h = "";
String m = "";
String s = "";
String ngay = "";
String thang = "";
String nam = "";
String hmdmoy = "";
String tmp = "";
String datetime = "";
unsigned long  prevtime;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(d0, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, INPUT);
  pinMode(d8, OUTPUT);
  // Set outputs to LOW
  digitalWrite(d0, LOW);
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
  digitalWrite(d5, LOW);
  digitalWrite(d6, LOW);
  digitalWrite(d7, LOW);
  digitalWrite(d8, LOW);

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
  if (! rtc.begin())
 {
   Serial.print("Couldn't find RTC");
   while (1);
 }

  if (! rtc.lostPower())
 {
   Serial.print("RTC is NOT running!");
   Serial.println();
 }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
}

void loop(){
  String h,m, ngay, thang,nam;
     DateTime now = rtc.now();
// Time
     if(now.hour()<=9)
     {
       h = "0" + (String)now.hour();
     }
     else {
       h = (String)now.hour();
     }
     if(now.minute()<=9)
     {
       m = "0" + (String)now.minute();
     }
     else {
       m = (String)now.minute();
     }
     if(now.second()<=9)
     {
       s = "0" + (String)now.second();
     }
     else {
       s = (String)now.second();
     }
 // Date
//    Serial.println(now.day());
//    Serial.println(now.month());
//    Serial.println(now.year());
      if(now.day()<=9)
     {
       ngay = "0" + (String)now.day();
     }
     else {
      ngay = (String)now.day();
     }
     if(now.month()<=9)
     {
       thang = "0" + (String)now.month();
     }
     else {
       thang = (String)now.month();
     }
     if(now.year()<=9)
     {
       nam = "0" + (String)now.year();
     }
     else {
      nam = (String)now.year();
     } 
     hmdmoy = nam + "-" + thang + "-" + ngay + " " + h + ":" + m;
     datetime = nam + "-" + thang + "-" + ngay + " " + h + ":" + m + ":" + s;
     tmp = (String)(rtc.getTemperature() + (String)" C");
     Serial.println(hmdmoy);
     // get nhiet do
//     Firebase.setString("Khu/WEATHER/TEMPERATURE", tmp); 
//     Firebase.setString("Khu/WEATHER/DATETIME", datetime);
// Connect To Firebase to getData
    fireStatus0 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS0/STATUS");                                      // get ld status input from firebase
    turnon0 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS0/TURNON");
    turnoff0 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS0/TURNOFF");
    if (fireStatus0 == "ON") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led0 Turned ON");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
      digitalWrite(d0, HIGH);                                                         // make external led ON
    } 
    else if (fireStatus0 == "OFF") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led0 Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
      digitalWrite(d0, LOW);                                                         // make external led OFF
    }
    else {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }
// Turn on led with settime
    if(hmdmoy == turnon0){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS0/STATUS", "ON");   
    }
    if(hmdmoy == turnoff0){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS0/STATUS", "OFF");        
    }
//============================================================================
//    fireStatus1 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS1/STATUS");                                      // get ld status input from firebase
//    turnon1 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS1/TURNON");
//    turnoff1 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS1/TURNOFF");
//    if (fireStatus1 == "ON") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led1 Turned ON");                         
//      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
//      digitalWrite(d1, HIGH);                                                         // make external led ON
//    } 
//    else if (fireStatus1 == "OFF") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led1 Turned OFF");
//      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
//      digitalWrite(d1, LOW);                                                         // make external led OFF
//    }
//    else {
//      Serial.println("Wrong Credential! Please send ON/OFF");
//    }
//    if(hmdmoy == turnon1){
//      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS1/STATUS", "ON");   
//    }
//    if(hmdmoy == turnoff0){
//      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS1/STATUS", "OFF");        
//    }
////============================================================================
//    fireStatus2 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS2/STATUS");                                      // get ld status input from firebase
//    turnon2 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS2/TURNON");
//    turnoff2 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS2/TURNOFF");
//    if (fireStatus2 == "ON") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led2 Turned ON");                         
//      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
//      digitalWrite(d2, HIGH);                                                         // make external led ON
//    } 
//    else if (fireStatus2 == "OFF") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led2 Turned OFF");
//      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
//      digitalWrite(d2, LOW);                                                         // make external led OFF
//    }
//    else {
//      Serial.println("Wrong Credential! Please send ON/OFF");
//    }
//    if(hmdmoy == turnon2){
//      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS2/STATUS", "ON");   
//    }
//    if(hmdmoy == turnoff0){
//      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS2/STATUS", "OFF");        
//    }
    //============================================================================
    fireStatus3 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS3/STATUS");                                      // get ld status input from firebase
    turnon3 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS3/TURNON");
    turnoff3 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS3/TURNOFF");
    if (fireStatus3 == "ON") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led3 Turned ON");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
      digitalWrite(d3, HIGH);                                                         // make external led ON
    } 
    else if (fireStatus3 == "OFF") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led3 Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
      digitalWrite(d3, LOW);                                                         // make external led OFF
    }
    else {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }
    if(hmdmoy == turnon3){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS3/STATUS", "ON");   
    }
    if(hmdmoy == turnoff3){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS3/STATUS", "OFF");        
    }
      //============================================================================
    fireStatus5 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS5/STATUS");                                      // get ld status input from firebase
    turnon5 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS5/TURNON");
    turnoff5 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS5/TURNOFF");
    if (fireStatus5 == "ON") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led5 Turned ON");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
      digitalWrite(d5, HIGH);                                                         // make external led ON
    } 
    else if (fireStatus5 == "OFF") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led5 Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
      digitalWrite(d5, LOW);                                                         // make external led OFF
    }
    else {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }
    if(hmdmoy == turnon5){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS5/STATUS", "ON");   
    }
    if(hmdmoy == turnoff5){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS5/STATUS", "OFF");        
    }
      //============================================================================
    fireStatus6 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS6/STATUS");                                      // get ld status input from firebase
    turnon6 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS6/TURNON");
    turnoff6 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS6/TURNOFF");
    if (fireStatus6 == "ON") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led6 Turned ON");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
      digitalWrite(d6, HIGH);                                                         // make external led ON
    } 
    else if (fireStatus6 == "OFF") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led6 Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
      digitalWrite(d6, LOW);                                                         // make external led OFF
    }
    else {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }
    if(hmdmoy == turnon6){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS6/STATUS", "ON");   
    }
    if(hmdmoy == turnoff6){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS6/STATUS", "OFF");        
    }
     //============================================================================
//    fireStatus7 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS7/STATUS");                                      // get ld status input from firebase
//    turnon7 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS7/TURNON");
//    turnoff7 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS7/TURNOFF");
//    if (fireStatus7 == "ON") {                                                          // compare the input of led status received from firebase
////      Serial.println("Led7 Turned ON");                         
//      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
//      digitalWrite(d7, HIGH);                                                         // make external led ON
//    } 
//    else if (fireStatus7 == "OFF") {                                                  // compare the input of led status received from firebase
////      Serial.println("Led7 Turned OFF");
//      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
//      digitalWrite(d7, LOW);                                                         // make external led OFF
//    }
//    else {
//      Serial.println("Wrong Credential! Please send ON/OFF");
//    }
//    if(hmdmoy == turnon7){
//      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS7/STATUS", "ON");   
//    }
//    if(hmdmoy == turnoff7){
//      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS7/STATUS", "OFF");        
//    }
// ==========================================================================
  fireStatus8 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS8/STATUS");                                      // get ld status input from firebase
  turnon8 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS8/TURNON");
  turnoff8 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS8/TURNOFF");
    if (fireStatus8 == "ON") {                                                          // compare the input of led status received from firebase
      // DOC GIA TRI CUA CHAN SO 13
      int value = digitalRead(d7);
//      Serial.println("Led8 Turned ON");                         
//      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
//      digitalWrite(d8, HIGH);                                                         // make external led ON
      if (value == HIGH){
      //    Serial.printf("High");
        digitalWrite(d8, HIGH);   
      }
      else {
          //digitalWrite(LED, LOW);
      //    Serial.printf("Low");
        digitalWrite(d8, LOW);
          
      } 
    } 
    else if (fireStatus8 == "OFF") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led8 Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
      digitalWrite(d8, LOW);                                                         // make external led OFF
    }
    else {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }
    if(hmdmoy == turnon8){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS8/STATUS", "ON");   
    }
    if(hmdmoy == turnoff8){
      Firebase.setString("90DODOCLOC/Phong1/LED_STATUS8/STATUS", "OFF");        
    }
}
