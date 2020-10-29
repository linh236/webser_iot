/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>  
#include <Wire.h>
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

String firestatus0 = "";
String turnon0 = "";
String turnoff0 = "";
String firestatus1 = "";
String turnon1 = "";
String turnoff1 = "";
String firestatus2 = "";
String turnon2 = "";
String turnoff2 = "";
String firestatus3 = "";
String turnon3 = "";
String turnoff3 = "";
String firestatus5 = "";
String turnon5 = "";
String turnoff5 = "";
String firestatus6 = "";
String turnon6 = "";
String turnoff6 = "";
String firestatus7 = "";
String turnon7 = "";
String turnoff7 = "";
String firestatus8 = "";
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
  pinMode(d7, OUTPUT);
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
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
}

void loop(){
    String house_room = "90DODOCLOC/Phong1/";
    firestatus0 = Firebase.getString(house_room+"led_status0/status");                                      // get ld status input from firebase
    turnon0 = Firebase.getString(house_room+"led_status0/turnon");
    turnoff0 = Firebase.getString(house_room+"led_status0/turnoff");
    if (firestatus0 == "on") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led0 Turned on");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led on
      digitalWrite(d0, HIGH);                                                         // make external led on
    } 
    else if (firestatus0 == "off") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led0 Turned off");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led off
      digitalWrite(d0, LOW);                                                         // make external led off
    }
    else {
      Serial.println("Wrong Credential! Please send on/off");
    }
// Turn on led with settime
//============================================================================
    firestatus1 = Firebase.getString(house_room+"led_status1/status");                                      // get ld status input from firebase
    turnon1 = Firebase.getString(house_room+"led_status1/turnon");
    turnoff1 = Firebase.getString(house_room+"led_status1/turnoff");
    if (firestatus1 == "on") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led1 Turned on");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led on
      digitalWrite(d1, HIGH);                                                         // make external led on
    } 
    else if (firestatus1 == "off") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led1 Turned off");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led off
      digitalWrite(d1, LOW);                                                         // make external led off
    }
    else {
      Serial.println("Wrong Credential! Please send on/off");
    }
   
////============================================================================
    firestatus2 = Firebase.getString(house_room+"led_status2/status");                                      // get ld status input from firebase
    turnon2 = Firebase.getString(house_room+"led_status2/turnon");
    turnoff2 = Firebase.getString(house_room+"led_status2/turnoff");
    if (firestatus2 == "on") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led2 Turned on");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led on
      digitalWrite(d2, HIGH);                                                         // make external led on
    } 
    else if (firestatus2 == "off") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led2 Turned off");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led off
      digitalWrite(d2, LOW);                                                         // make external led off
    }
    else {
      Serial.println("Wrong Credential! Please send on/off");
    }
   
    //============================================================================
    firestatus3 = Firebase.getString(house_room+"led_status3/status");                                      // get ld status input from firebase
    turnon3 = Firebase.getString(house_room+"led_status3/turnon");
    turnoff3 = Firebase.getString(house_room+"led_status3/turnoff");
    if (firestatus3 == "on") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led3 Turned on");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led on
      digitalWrite(d3, HIGH);                                                         // make external led on
    } 
    else if (firestatus3 == "off") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led3 Turned off");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led off
      digitalWrite(d3, LOW);                                                         // make external led off
    }
    else {
      Serial.println("Wrong Credential! Please send on/off");
    }

      //============================================================================
    firestatus5 = Firebase.getString(house_room+"led_status5/status");                                      // get ld status input from firebase
    turnon5 = Firebase.getString(house_room+"led_status5/turnon");
    turnoff5 = Firebase.getString(house_room+"led_status5/turnoff");
    if (firestatus5 == "on") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led5 Turned on");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led on
      digitalWrite(d5, HIGH);                                                         // make external led on
    } 
    else if (firestatus5 == "off") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led5 Turned off");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led off
      digitalWrite(d5, LOW);                                                         // make external led off
    }
    else {
      Serial.println("Wrong Credential! Please send on/off");
    }
   
      //============================================================================
    firestatus6 = Firebase.getString(house_room+"led_status6/status");                                      // get ld status input from firebase
    turnon6 = Firebase.getString(house_room+"led_status6/turnon");
    turnoff6 = Firebase.getString(house_room+"led_status6/turnoff");
    if (firestatus6 == "on") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led6 Turned on");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led on
      digitalWrite(d6, HIGH);                                                         // make external led on
    } 
    else if (firestatus6 == "off") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led6 Turned off");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led off
      digitalWrite(d6, LOW);                                                         // make external led off
    }
    else {
      Serial.println("Wrong Credential! Please send on/off");
    }
 
     //============================================================================
    firestatus7 = Firebase.getString(house_room+"led_status7/status");                                      // get ld status input from firebase
    turnon7 = Firebase.getString(house_room+"led_status7/turnon");
    turnoff7 = Firebase.getString(house_room+"led_status7/turnoff");
    if (firestatus7 == "on") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led7 Turned on");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led on
      digitalWrite(d7, HIGH);                                                         // make external led on
    } 
    else if (firestatus7 == "off") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led7 Turned off");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led off
      digitalWrite(d7, LOW);                                                         // make external led off
    }
    else {
      Serial.println("Wrong Credential! Please send on/off");
    }
   
// ==========================================================================
  firestatus8 = Firebase.getString(house_room+"led_status8/status");                                      // get ld status input from firebase
  turnon8 = Firebase.getString(house_room+"led_status8/turnon");
  turnoff8 = Firebase.getString(house_room+"led_status8/turnoff");
      if (firestatus8 == "on") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led8 Turned on");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led on
      digitalWrite(d8, HIGH);                                                         // make external led on
    } 
    else if (firestatus8 == "off") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led8 Turned off");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led off
      digitalWrite(d8, LOW);                                                         // make external led off
    }
    else {
      Serial.println("Wrong Credential! Please send on/off");
    }
}
