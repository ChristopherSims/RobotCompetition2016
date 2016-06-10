#include <HID.h>
//Libraries
#include <Servo.h>
//Servo Variables
int SearchDelay = 1000;
Servo MyServo; int pos = 90; char Direction = 'R';   int R;
 int ServoPin = 9;
 int SpinyPin = 7;
// Edge Detection Pins
int FrontLeft = 0; //A0
 int FrontRight = 1; // A1
 int BackLeft = 2; //A2
 int BackRight = 3;//A3
// Object Detection
int IRSensor = 4; //A4
// Kill Switch
int KillSwitch = 6; //6
/// Digital Read Lasers
 int LaserPrime = 5; //0
 int LaserRotate= 12; //1
float Threshold = 4; // voltage
 /*
  * Motor Control Variables
  */
  int SpeedPin = 5;
  int MotorLeftDirection = 11;
  int MotorRightDirection = 10;
int Speed = 1000;
char STATE = 'F';
/*
 * Search Algoritm
 */
 int Timer = 0;
 int Milliseconds = 10;
 int MAXTIMER = 1000/Milliseconds;
bool Search =false;
 bool RUN = true;
void setup() {
   pos = 90; Direction = 'L'; Timer = 0; STATE = 'F';
  Serial.begin(9600);
  // put your setup code here, to run once:
  MyServo.attach(ServoPin);
  pinMode(FrontLeft, INPUT);
  pinMode(FrontRight, INPUT);
  pinMode(BackLeft, INPUT);
  pinMode(BackRight,INPUT);
  pinMode(MotorLeftDirection,OUTPUT);
  pinMode(MotorRightDirection,OUTPUT);
  pinMode(KillSwitch, INPUT);

  pinMode(SpinyPin,OUTPUT);
  pinMode(IRSensor,INPUT);
  //// CUP Detection
  pinMode(LaserPrime,INPUT);
  pinMode(LaserRotate,INPUT);
}

float READ_PIN(int pin){
  int SensorValue = analogRead(pin);
  float value =  SensorValue*(5.0/1023.0);
 return value;
}
void DETECT_EDGE(){
  // Front left detection
 float val = READ_PIN(BackRight);

if( READ_PIN(FrontLeft) < 3.2){
  Serial.print("Edge Front left");
    Serial.println();
  STATE = 'R';
  DRIVE();delay(1000);
}
// Front Right Detection
else if( READ_PIN(FrontRight) < 3.2){
  Serial.print("Edge front right");
    Serial.println();
  STATE = 'L';
  DRIVE();delay(1000);
}
else if( READ_PIN(BackRight) < 3.2){
  Serial.print("edge Back Right ");
    Serial.println();
  STATE = 'F';
  DRIVE();delay(1000);
  STATE = 'L';
  DRIVE();delay(1000);
}
else if( READ_PIN(BackLeft) < 3.2){
  Serial.print("Edge Back left");
    Serial.println();
  STATE = 'F';
  DRIVE();delay(1000);
  STATE = 'R';
  DRIVE();delay(1000);
}
//Serial.println(val);
}

void DRIVE(){
 if (STATE == 'F'){ //Forward
 digitalWrite(MotorLeftDirection,HIGH);
 digitalWrite(MotorRightDirection,HIGH);
 }
 else if(STATE== 'L'){ // LEFT
 digitalWrite(MotorLeftDirection,HIGH);
 digitalWrite(MotorRightDirection,LOW);
 }
 else if(STATE == 'R'){ //Right
digitalWrite(MotorLeftDirection,LOW);
 digitalWrite(MotorRightDirection,HIGH);
 }
 else if(STATE == 'B'){ // BACKWARD
   digitalWrite(MotorLeftDirection,LOW);
 digitalWrite(MotorRightDirection,LOW);
 }
analogWrite(SpeedPin,Speed);
}
  /*
   * Servo Control
   */
void ServoControl(){
    MyServo.write(pos); // set servo
   if (Direction == 'R'){
   pos++;
   if (pos >= 140){
    Direction = 'L';
   }
 }
if (Direction =='L') { 
  pos--;
   if (pos <= 40){ 
    Direction = 'R';
   }
}
}
void CupDetection(){
  int DA = pos - 180;
  float val = READ_PIN(LaserPrime);
  if ((READ_PIN(LaserPrime) > 4.6) || (READ_PIN(LaserRotate) > 4.6)){
    STATE = 'F';
     DRIVE();
      Search = false;
    Serial.print("Found it!");
    Serial.println();
 
  }
}
  void ResetTimer(){
      if(( READ_PIN(LaserRotate) < 4) || READ_PIN(LaserRotate) < 4){
    Search = true;
  }
  }
  /*
   * Search Algorithm
   * 
   * 
   */
 
 void SEARCH(){ 
    Serial.print("Searching!");
    Serial.println();
  R = random(100);
  if(R > 25) STATE = 'R';
  else STATE = 'F';
  ResetTimer();
 }
 
void loop() {
  if (Search == true) {
    SEARCH();
  }
   //ServoControl();
   digitalWrite(SpinyPin,HIGH);
  // DETECT_EDGE();
   CupDetection();
   DRIVE();
   Serial.println(LaserRotate);
   }
