#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>//thư viện giao tiếp 
// Replace with your network credentials
#include <FirebaseArduino.h>
const char* ssid = "Simon Phu";
const char* password = "Qu@ngphu92";
#define FIREBASE_HOST "smartpro-a82e8.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "zdcJHBdXwSjMcWyX4QEGDFzOCWVLNgib49bj9ce4"

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String modevan1 = "off1";
String modevan2 = "off2";
String modevan3 = "off3";
String modevan4 = "off4";
String mode1 = "auto";
String modebom = "batbom";

String pushstatus = "";
String pullstatus = "";
String ledstatus = "";
String tmp = "";
// Assign output variables to GPIO pins
String a, b, c, d; //biến lưu giá trị các text lên server


String myString; // chuỗi trung gian để hoàn thành việc nhận chưỡi ký tự để sau đó thực hiện quá trìn cắt
char rdata; //biến nhận kí tự từ arduino

int led1, led2, led3, led4; // biến để lưu giá trị cảm biến 1,2,3,4 sau khi cắt chuỗi ra sau các dấ ","



const int bom = 4;//D2
const int van1 = 5;//D1
const int van2 = 16;//D0
const int van3 = 12;//D6
const int van4 = 14; //D5

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {



  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(bom, OUTPUT);
  pinMode(van1, OUTPUT);
  pinMode(van3, OUTPUT);
  pinMode(van4, OUTPUT);
  pinMode(van2, OUTPUT);
  // Set outputs to LOW


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
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void loop() {

  if (Serial.available() > 0 )
  {
    rdata = Serial.read();//nhận ký tự từ arduino
    myString = myString + rdata; // đưa vào chuỗi myString
    if ( rdata == '\n')
    {

      String l = getValue(myString, ',', 0);// tách giá trị cảm biến 1
      String m = getValue(myString, ',', 1);// tách giá trị cảm biến 2 sau dấu "'" thứ 1
      String n = getValue(myString, ',', 2);// tách giá trị cảm biến 3 sau dấu "'" thứ 2
      String o = getValue(myString, ',', 3);// tách giá trị cảm biến 4 sau dấu "'" thứ 3

      led1 = l.toInt();//lưu giá trị của cảm biến 1 sau khi cắt chuỗi bằng kiểu nguyên
      led2 = m.toInt();//lưu giá trị của cảm biến 2 sau khi cắt chuỗi bằng kiểu nguyên
      led3 = n.toInt();//lưu giá trị của cảm biến 3 sau khi cắt chuỗi bằng kiểu nguyên
      led4 = o.toInt();//lưu giá trị của cảm biến 4 sau khi cắt chuỗi bằng kiểu nguyên

      myString = "";//cho chuỗi về rổng để tiếp tục chu trình
    }
  }
  int chartValue1 = map(led1, -30, 1023, 0, 100); //biến đổi từ thang đo analog sang % độ ẩm cb1
  int chartValue2 = map(led2, -30, 1023, 0, 100);
  int chartValue3 = map(led3, -30, 1023, 0, 100);
  int chartValue4 = map(led4, -30, 1023, 0, 100);
  chartValue1 = 100 - chartValue1;//độ ẩm thực tế 1
  chartValue2 = 100 - chartValue2;
  chartValue3 = 100 - chartValue3;
  chartValue4 = 100 - chartValue4;

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();



            // turns the GPIOs on and off

            if (header.indexOf("GET /auto") >= 0) {

              mode1 = "auto";

            } else if (header.indexOf("GET /manual") >= 0) {

              mode1 = "manual";

            }
            if (header.indexOf("GET /batbom") >= 0) {

              modebom = "batbom";

            } else if (header.indexOf("GET /tatbom") >= 0) {

              modebom = "tatbom";

            }

            if (header.indexOf("GET /on1") >= 0) {

              modevan1 = "on1";

            } else if (header.indexOf("GET /off1") >= 0) {

              modevan1 = "off1";

            }

            if (header.indexOf("GET /on2") >= 0) {

              modevan2 = "on2";

            } else if (header.indexOf("GET /off2") >= 0) {

              modevan2 = "off2";

            }
            if (header.indexOf("GET /on3") >= 0) {

              modevan3 = "on3";

            } else if (header.indexOf("GET /off3") >= 0) {

              modevan3 = "off3";

            }
            if (header.indexOf("GET /on4") >= 0) {

              modevan4 = "on4";

            } else if (header.indexOf("GET /off4") >= 0) {

              modevan4 = "off4";

            }
           
            tmp = Firebase.getString("vuonrau/tmp");
            ledstatus = Firebase.getString("vuonrau/led/status");
            pushstatus = Firebase.getString("vuonrau/push/status");
            pullstatus = Firebase.getString("vuonrau/pull/status");    

            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head>");
            client.println("<title>Đồ Án </title>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<meta charset='utf-8'>");
            client.println("<meta http-equiv='refresh' content='5'>");
            client.println("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'>");
            client.println("<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js'></script>");
            client.println("<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.16.0/umd/popper.min.js'></script>");
            client.println(" <script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js'></script>");
            client.println("</head>");
            client.println("<body>");
            client.println("<div class='container'>");
            client.println("<div class='row'><div class='col-sm-12 bg-dark'><img class='w-100 img_logo' src ='https://kdientu.duytan.edu.vn/images/icon/logo_fo.png'/></div></div>");
            client.println("<div class='row mt-4'><h3 class='col-sm-12'>ĐỀ TÀI: HỆ HÔNG VƯỜN RAU THÔNG MINH ĐIỀU KHIỂN QUA MẠNG INTERNET</h3></div>");
            client.println("<div class='col-sm-12'><b>SVTH: LÊ TRUNG NGHĨA</b></div>");
            client.println("<div class='col-sm-12'><b>GVHD: THS. NGUYỄN THẾ TÂM</b></div>");
            client.println("<div class='container'>");
            client.println("<div class='float-right'><h5>Nhiệt độ: ");
              client.println(tmp);
              client.println(" °C");
            client.println("</h5></div>");
            ///////////////////////////////////////////manual////////////////////////////////////////////////////////////////////
            client.println("<table class='col-sm-6 table table-bordered'>");
              client.println("<tr>");
                client.println("<td>Đẩy mái che</td>");
                client.println("<td class='text-center'>");
                  client.println(pushstatus == "on" ? "Run" : "Stop");
                client.println("</td>");
               client.println("</tr>");
               client.println("<tr>");
                client.println("<td>Kéo mái che</td>");
                client.println("<td class='text-center'>");
                  client.println(pullstatus == "on" ? "Run" : "Stop");
                client.println("</td>");
               client.println("</tr>");
               client.println("<tr>");
                client.println("<td>Bóng đèn</td>");
                client.println("<td class='text-center'>");
                  client.println(ledstatus == "on" ? "Run" : "Stop");
                client.println("</td>");
               client.println("</tr>");
            client.println("<tr>");
            client.println("<td>Chế Độ/Mode</td>");
            if (mode1 == "auto") {
              client.println("<td class='text-center'><a class='btn btn-sm btn-primary pb-1 pl-4 pr-4' href='/manual'>Auto</a></td>");
            } else {
              client.println("<td class='text-center'><a class='btn btn-sm btn-primary pb-1 pt-1 pl-3 pr-3' href=\"/auto\"\" >Manual</a></td>");
            }
            client.println("</tr>");
            if (mode1 == "manual") {
              client.println("<tr>");
              client.println("<td><div>BƠM/PUMP:</div></td>");
              if (modebom == "batbom") {
                digitalWrite(bom, HIGH );
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/tatbom\"\">Stop BƠM</a></td>");
              } else {
                digitalWrite(bom, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/batbom\"\">Run BƠM</a></td>");
              }
              client.println("</tr>");
              client.println("</table>");
              client.println("<table class='table table-bordered'>");
              client.println("<tr>");
              client.println("<td><div>Độ Ẩm Khu Vực 1:</div></td>");
              client.println("<td class='text-center'>");
              client.println(chartValue1);
              client.println(" %");
              client.println("</td>");
              client.println("<td><div>Van_1/Valve1_1:</div></td>");
              if (modevan1 == "off1") {
                digitalWrite(van1, HIGH);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/on1\"\" >Stop Vavel_1</a></td>");
              } else {
                digitalWrite(van1, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/off1\"\" >Run Vavel_1</a></td>");
              }
              client.println("</tr>");
              client.println("<tr>");
              client.println("<td><div>Độ Ẩm Khu Vực 2:</div></td>");
              client.println("<td class='text-center'>");
              client.println(chartValue2);
              client.println(" %");
              client.println("</td>");
              client.println("<td><div >Van_2/Valve1_2:</div></td>");
              if (modevan2 == "off2") {
                digitalWrite(van2, HIGH);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/on2\"\" >Stop Vavel_2</a></td>");
              } else {
                digitalWrite(van2, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/off2\"\">Run Vavel_2</a></td>");
              }
              client.println("</tr>");
              client.println("<tr>");
              client.println("<td><div>Độ Ẩm Khu Vực 3:</div></td>");
              client.println("<td class='text-center'>");
              client.println(chartValue3);
              client.println(" %");
              client.println("</td>");
              client.println("<td><div>Van_3/Valve1_3:</div></td>");
              if (modevan3 == "off3") {
                digitalWrite(van3, HIGH);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/on3\"\">Stop Vavel_3</a></td>");
              } else {
                digitalWrite(van3, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/off3\"\">Run Vavel_3</a></td>");
              }
              client.println("</tr>");
              client.println("<tr>");
              client.println("<td><div>Độ Ẩm Khu Vực 4:</div></td>");
              client.println("<td class='text-center'>");
              client.println(chartValue4);
              client.println(" %");
              client.println("</td>");
              client.println("<td><div >Van_4/Valve1_4:</div></td>");
              if (modevan4 == "off4") {
                digitalWrite(van4, HIGH);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/on4\"\" >Stop Vavel_4</a></td>");
              } else {
                digitalWrite(van4, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"/off4\"\" >Run Vavel_4</a></td>");
              }
              client.println("</tr></table>");
            }
            ////////////////////////////////////////////////////////auto//////////

            client.println("<tr>");
            if (mode1 == "auto") {
              client.println("<td><div>BƠM/PUMP:</div></td>");
              if (chartValue1 < 50 || chartValue2 < 50 || chartValue3 < 50 || chartValue4 < 50) {
                digitalWrite(bom, HIGH);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Run BƠM</a></td>");
              } else {
                digitalWrite(bom, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Stop BƠM</a></td>");
              }
              client.println("</tr>");
              client.println("<table class='table table-bordered'>");
              client.println("<tr>");
              client.println("<td><div >Độ Ẩm Khu Vực 1:</div></td>");
              client.println("<td class='text-center'>");
              client.println(chartValue1);
              client.println(" %");
              client.println("</td>");
              client.println("<td><div >Van_1/Valve1_1:</div></td>");
              if (chartValue1 < 50) {
                digitalWrite(van1, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Run Vavel_1</a></td>");
              }
              else {
                digitalWrite(van1, HIGH);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Stop Vavel_1</a></td>");
              }
              client.println("</tr>");
              client.println("<tr>");
              client.println("<td><div >Độ Ẩm Khu Vực 2:</div></td>");
              client.println("<td class='text-center'>");
              client.println(chartValue2);
              client.println(" %");
              client.println("</td>");
              client.println("<td><div>Van_2/Valve1_2:</div></td>");
              if (chartValue2 < 50) {
                digitalWrite(van2, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Run Vavel_2</a></td>");
              } else {
                digitalWrite(van2, HIGH);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Stop Vavel_2</a></td>");
              }
              client.println("</tr>");
              client.println("<tr>");
              client.println("<td><div >Độ Ẩm Khu Vực 3:</div></td>");
              client.println("<td class='text-center'>");
              client.println(chartValue3);
              client.println(" %");
              client.println("</td>");
              client.println("<td><div >Van_3/Valve1_3:</div></td>");
              if (chartValue3 < 50) {
                digitalWrite(van3, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Run Vavel_3</a></td>");
              } else {
                digitalWrite(van3, HIGH);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Stop Vavel_3</a></td>");
              }
              client.println("</tr>");
              client.println("<tr>");
              client.println("<td><div >Độ Ẩm Khu Vực 4:</div></td>");
              client.println("<td class='text-center'>");
              client.println(chartValue4);
              client.println(" %");
              client.println("</td>");
              client.println("<td><div>Van_4/Valve1_4:</div></td>");
              if (chartValue4 < 50) {
                digitalWrite(van4, LOW);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Run Vavel_4</a></td>");
              } else {
                digitalWrite(van4, HIGH);
                client.println("<td class='text-center'><a class='btn btn-sm btn-primary' href=\"\" >Stop Vavel_4</a></td>");
              }
              client.println("</tr>");
            }

            ////////////////////////////////////////////////////////////////////
            client.println("</div></div></table>");
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
