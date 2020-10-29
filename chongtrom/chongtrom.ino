int PIR = 13;
int BUZZER = 15;


void setup() {
  Serial.begin(115200);
  //define the Relay pin as output
  //define the sensor pin as input
  pinMode(BUZZER, OUTPUT);
  pinMode(PIR, INPUT);
}

void loop() {
  //using the digitalRead function we will read the signal of the sensor
  int value = digitalRead(PIR);
  //if its high or if an any object is detected it will activate the Relay Module
  if (value == HIGH){
//    Serial.printf("High");
    digitalWrite(BUZZER, HIGH); 
   
  }
  else {
    //digitalWrite(LED, LOW);
//    Serial.printf("Low");
    digitalWrite(BUZZER, LOW);
    
  } 
}
