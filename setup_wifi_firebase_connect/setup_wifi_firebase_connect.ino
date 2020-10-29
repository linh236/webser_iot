/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "EEPROM.h"
#include <FirebaseArduino.h>  
#include <Wire.h>
//DS3232RTC rtc;
// wifi
//const char* ssid     = "NEWS";
//const char* password = "1651120032";
//const char* host = "testiot-d9f74.firebaseio.com/";          
//const char* auth = "4Ku3ygUrZcLyXfj6QL0EpbNxB0q1RUiZFRvwPYwr" ;   
#define FIREBASE_HOST "iotpro-58c44.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "F4mMmNXp1CPYvJYX5KwtrLifqw6UvVO4fyCUKhoj" 

  // setup webserver
ESP8266WebServer server(80);
const char*     ssid           = "XSwitch";
const char*     passphrase     = "123456789";

int             inRelay1       = 14; //Chan GPIO14 gắn vào relay
String          st;
String          content;
int             statusCode;
 
void writeResponse(char *response) { Serial.print(response); Serial.print("\n"); }
void (*writeCallback)(char *str) = writeResponse;

//----------------------------------------------------------------------------------------
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
//----------------------------------------------------
//  webserver
  pinMode(inRelay1,OUTPUT);
    EEPROM.begin(512);
    delay(10);
    Serial.println("Startup");
    // read eeprom for ssid, pass and blynk
    Serial.println("Reading EEPROM ssid");
    String esid;
    for (int i = 0; i < 32; ++i)
    {
        esid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(esid.c_str());
    esid.trim();
 
    Serial.println("Reading EEPROM pass");
    String epass = "";
    for (int i = 32; i < 96; ++i)
    {
        epass += char(EEPROM.read(i));
    }
    Serial.print("PASS: ");
    Serial.println(epass.c_str());
    epass.trim();

    if ( esid.length() > 1 )
    {
        WiFi.begin(esid.c_str(), epass.c_str());
        Serial.print(testWifi());
        if (testWifi())
        {
            launchWeb(0);
            WiFi.disconnect();
            EEPROM.end();
            return;
        }
    }
    setupAP();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);    
    EEPROM.end();
//--------------------------------------------------------------

    
  // Connect to Wi-Fi network with SSID and password
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  // Print local IP address and start web server
//  Serial.println("");
//  Serial.println("WiFi connected.");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
}
bool testWifi(void)
{
    int c = 0;
    Serial.println("Xin vui long doi ket noi WIFI");
    while ( c < 20 )
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            return true;
        }
        delay(1000);
        Serial.print(WiFi.status());
        c++;
    }
    Serial.println("");
    Serial.println("Thoi gian ket noi cham, Mo AP");
    return false;
}
 
void launchWeb(int webtype)
{
    Serial.println("");
    Serial.println("WiFi ket noi");
    Serial.print("Dia chi IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    createWebServer(webtype);
    // Start the server
    server.begin();
    Serial.println("May chu bat dau");
}

void setupAP(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    Serial.println("Tim hoan tat");
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    st = "<ol>";
    for (int i = 0; i < n; ++i)
    {
        // Print SSID and RSSI for each network found
        st += "<li>";
        st += WiFi.SSID(i);
        st += " (";
        st += WiFi.RSSI(i);
        st += ")";
        st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
        st += "</li>";
    }
    st += "</ol>";
    delay(100);
    Serial.println("softap");
    Serial.println(ssid);
    Serial.println(passphrase);
    WiFi.softAP(ssid, passphrase, 6);
 
    launchWeb(1);
    Serial.println("over");
}

void createWebServer(int webtype)
{
    if ( webtype == 1 )
    {
        server.on("/", []()
        {
            IPAddress ip = WiFi.softAPIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            content = "<!DOCTYPE HTML>\r\n<html><h2>XSwitch</h2>";
            //content += ipStr;
            //content += "<form method='get' action='setting'><table width='100%' border='1'><tr><td width=\"30%\"><label>Wifi</label></td><td width=\"70%\><input name='ssid' length=32 width='500'></td></tr><tr><td><label>Password</label></td><td><input name='pass' length=64 width='500'></td></tr><tr><td><label>Blynk</label></td><td><input name='blynk' length=32 width='500'></td></tr><tr><td></td><td><input type='submit'></tr></tr></table></form>";
            content += "<form method=\"get\" action=\"setting\">";
            content += "<div>Wifi</div>";
            content += "<div><input name=\"ssid\" size=\"40\"></div>";
            content += "<div>Mat Khau</div>";
            content += "<div><input name=\"pass\" size=\"40\"></div>";
        
            content += "<div><input type='submit'></div>";
 
            content += "<p>";
            content += st;
            content += "</p>";
            content += "</html>";
            server.send(200, "text/html", content);
        });
        server.on("/setting", []()
        {
            String qsid = server.arg("ssid");
            String qpass = server.arg("pass");
            
            if (qsid.length() > 0 && qpass.length() > 0)
            {
                EEPROM.begin(512);
                Serial.println("clearing eeprom");
                for (int i = 0; i < 128; ++i)
                {
                    EEPROM.write(i, 0);
                }
                EEPROM.commit();
                Serial.println(qsid);
                Serial.println("");
                Serial.println(qpass);
                Serial.println("");
                
 
                Serial.println("writing eeprom ssid:");
                for (int i = 0; i < qsid.length(); ++i)
                {
                    EEPROM.write(i, qsid[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qsid[i]);
                }
 
                Serial.println("writing eeprom pass:");
                for (int i = 0; i < qpass.length(); ++i)
                {
                    EEPROM.write(32 + i, qpass[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qpass[i]);
                }
 
               
                EEPROM.commit();
                EEPROM.end();
              
                content = "{\"Success\":\"Luu vao he thong. Khoi dong lai ten wifi moi\"}";
                statusCode = 200;
            }
            else
            {
                content = "{\"Error\":\"404 not found\"}";
                statusCode = 404;
                Serial.println("Sending 404");
            }
            server.send(statusCode, "application/json", content);
        });
    }
    else if (webtype == 0)
    {
        server.on("/", []()
        {
            IPAddress ip = WiFi.localIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            server.send(200, "application/json", "{\"IP\":\"" + ipStr + "\"}");
        });
        server.on("/cleareeprom", []()
        {
            content = "<!DOCTYPE HTML>\r\n<html>";
            content += "<h2>XSwitch</h2><p>Clearing the EEPROM</p></html>";
            server.send(200, "text/html", content);
            Serial.println("clearing eeprom");
            for (int i = 0; i < 128; ++i)
            {
                EEPROM.write(i, 0);
            }
                      
            EEPROM.commit();
        });
    }
}
 
 
void loop(){
   // handeClient webserver
    server.handleClient();
   //-------------------------
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
      //Serial.println("Wrong Credential! Please send ON/OFF");
    }
// Turn on led with settime
//============================================================================
    fireStatus1 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS1/STATUS");                                      // get ld status input from firebase
    turnon1 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS1/TURNON");
    turnoff1 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS1/TURNOFF");
    if (fireStatus1 == "ON") {                                                          // compare the input of led status received from firebase
      Serial.println("Led1 Turned ON");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
      digitalWrite(d1, HIGH);                                                         // make external led ON
    } 
    else if (fireStatus1 == "OFF") {                                                  // compare the input of led status received from firebase
      Serial.println("Led1 Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
      digitalWrite(d1, LOW);                                                         // make external led OFF
    }
    else {
      //Serial.println("Wrong Credential! Please send ON/OFF");
    }
   
////============================================================================
    fireStatus2 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS2/STATUS");                                      // get ld status input from firebase
    turnon2 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS2/TURNON");
    turnoff2 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS2/TURNOFF");
    if (fireStatus2 == "ON") {                                                          // compare the input of led status received from firebase
      Serial.println("Led2 Turned ON");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
      digitalWrite(d2, HIGH);                                                         // make external led ON
    } 
    else if (fireStatus2 == "OFF") {                                                  // compare the input of led status received from firebase
      Serial.println("Led2 Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
      digitalWrite(d2, LOW);                                                         // make external led OFF
    }
    else {
      //Serial.println("Wrong Credential! Please send ON/OFF");
    }
   
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
      //Serial.println("Wrong Credential! Please send ON/OFF");
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
      //Serial.println("Wrong Credential! Please send ON/OFF");
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
      //Serial.println("Wrong Credential! Please send ON/OFF");
    }
 
     //============================================================================
    fireStatus7 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS7/STATUS");                                      // get ld status input from firebase
    turnon7 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS7/TURNON");
    turnoff7 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS7/TURNOFF");
    if (fireStatus7 == "ON") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led7 Turned ON");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
      digitalWrite(d7, HIGH);                                                         // make external led ON
    } 
    else if (fireStatus7 == "OFF") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led7 Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
      digitalWrite(d7, LOW);                                                         // make external led OFF
    }
    else {
      //Serial.println("Wrong Credential! Please send ON/OFF");
    }
   
// ==========================================================================
  fireStatus8 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS8/STATUS");                                      // get ld status input from firebase
  turnon8 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS8/TURNON");
  turnoff8 = Firebase.getString("90DODOCLOC/Phong1/LED_STATUS8/TURNOFF");
      if (fireStatus8 == "ON") {                                                          // compare the input of led status received from firebase
//      Serial.println("Led8 Turned ON");                         
      digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
      digitalWrite(d8, HIGH);                                                         // make external led ON
    } 
    else if (fireStatus8 == "OFF") {                                                  // compare the input of led status received from firebase
//      Serial.println("Led8 Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
      digitalWrite(d8, LOW);                                                         // make external led OFF
    }
    else {
      //Serial.println("Wrong Credential! Please send ON/OFF");
    }
}
