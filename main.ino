#include <WiFi.h>



// --- PIN DEFINITIONS ---
// const int enA = 33;
// const int in1 = 12;
// const int in2 = 19;
// const int in3 = 27;
// const int in4 = 14;
// const int enB = 18;

const int enA = 4;
const int in1 = 27;
const int in2 = 14;
const int in3 = 12;
const int in4 = 19;
const int enB = 18;

// damage 33


// const int enA = 32;
// const int in1 = 25;
// const int in2 = 26;
// const int in3 = 27;
// const int in4 = 14;
// const int enB = 13;

// --- WIFI SETTINGS ---
const char *ssid = "ESP32_RC_Car";      // Network Name
const char *password = "12341234";      // Password
WiFiServer server(80);                  // Port 80

int motorSpeed = 1023; 
int motorSpeedX = 0;
double yvalue = 511;
double xvalue = 511;
bool fwd = true;

void setup() {
  analogWriteResolution(enA, 10);
  analogWriteResolution(enB, 10);
  Serial.begin(115200);

  pinMode(enA, OUTPUT); pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT); pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  
  Serial.println("\n\n--- STARTING ---");
  WiFi.softAP(ssid, password);
  
  Serial.println("AP Created!");
  Serial.print("Connect your phone to: "); Serial.println(ssid);
  Serial.print("Then in App use IP: "); Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    
    while (client.connected()) {
      if (client.available()) {
        char command = client.read();

        
        Serial.print("App Sent: ");
        Serial.println(command); 

        if(command == 'L' || command == 'l'){
            turnLeft(); 
        }
        if(command == 'R' || command == 'r') {
            turnRight(); 
        }
        if(command == 'S' || command == 's'){
          stopCar();
        }

        if (command == 'Y' || command == 'y') {
          yvalue = client.parseInt(); 
          
          if (yvalue >= 0 && yvalue <= 1024) {
            if(yvalue<505){
              motorSpeed = 2*(511-yvalue);
              moveBackward();
              fwd = false;
            }
            else if(yvalue>515){
              motorSpeed = 2*(yvalue-511);
              moveForward();
              fwd = true;
              
            }
            else{
              motorSpeed = 0;
            }

            if(xvalue < 511){
              double ratio = xvalue/511;
              motorSpeedX = ratio*motorSpeed;
              if(fwd){
                moveLeftF();
              }
              else{
                moveLeftB();
              }
            }
            else if(xvalue > 515){
              double ratio = (1023-xvalue)/505;
              motorSpeedX = ratio*motorSpeed;
              if(fwd){
                moveRightF();
              }
              else{
                moveRightB();
              }
            }
            else{
              motorSpeedX = 0;
            }
          }
        }

        /////////////////////////////////////////////

        if (command == 'X' || command == 'x') {
          xvalue = client.parseInt();

          if(xvalue < 505){
            double ratio = xvalue/505;
            motorSpeedX = ratio*motorSpeed;
            if(fwd){
                moveLeftF();
              }
              else{
                moveLeftB();
              }
          }
          else if(xvalue > 515){
            double ratio = (1023-xvalue)/505;
            motorSpeedX = ratio*motorSpeed;
            if(fwd){
                moveRightF();
              }
              else{
                moveRightB();
              }
          }
          else{
            motorSpeedX = 0;
          }
        }
        
      }
    }
    if(!client.connected()){
      stopCar();
    }
    client.stop();
  }
}

// --- MOTOR FUNCTIONS ---
void moveForward() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW); analogWrite(enA, motorSpeed);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW); analogWrite(enB, motorSpeed);
}
void moveBackward() {
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(enA, motorSpeed);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enB, motorSpeed);
}
void turnLeft() {
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(enA, 1023);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW); analogWrite(enB, 1023);
}
void turnRight() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW); analogWrite(enA, 1023);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enB, 1023);
}
void stopCar() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW); analogWrite(enA, 0);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW); analogWrite(enB, 0);
}
void moveLeftF(){
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW); analogWrite(enA, motorSpeedX);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW); analogWrite(enB, motorSpeed);
}
void moveRightF(){
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW); analogWrite(enA, motorSpeed);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW); analogWrite(enB, motorSpeedX);
}
void moveLeftB(){
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(enA, motorSpeedX);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enB, motorSpeed);
}
void moveRightB(){
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(enA, motorSpeed);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enB, motorSpeedX);
}




