/*  
  Made for arduino mega
*/
//#include "gyro.h"
// Motor variables
// Driver Left
//front is viewed from the arduino out ports

// Front left
const byte enAL = 6; 
const byte in1L = 28; 
const byte in2L = 29; 
 
// Back left
const byte enBL = 5;
const byte in3L = 26; // Any digital
const byte in4L = 27; // Any digital

// Driver Right

// Front right
const byte enAR = 4;
const byte in1R = 24;
const byte in2R = 25;

// Back right
const byte enBR = 3;  
const byte in3R = 22; 
const byte in4R = 23; 
void setup(){
  Serial.begin(115200);
  Serial.println("Robotcar Running");
  Serial.println("Initializing motors");
 
  // Set all the motor control pins to outputs

  // Left motors
  pinMode(enAL, OUTPUT);
  pinMode(enBL, OUTPUT);
  pinMode(in1L, OUTPUT);
  pinMode(in2L, OUTPUT);
  pinMode(in3L, OUTPUT);
  pinMode(in4L, OUTPUT);

  // Right motors
  pinMode(enAR, OUTPUT);
  pinMode(enBR, OUTPUT);
  pinMode(in1R, OUTPUT);
  pinMode(in2R, OUTPUT);
  pinMode(in3R, OUTPUT);
  pinMode(in4R, OUTPUT);
  gyroSetup();
}
 
void loop(){
  
  driveMeccanumGyro(90, 70, 200);
  delay(1000);
  driveMeccanumGyro(270, 70, 200);
  delay(1000);

}
