#include "TimerOne.h"

double diskslots = 24;

const byte frontLeftPin = 18;
unsigned int frontLeftCounter = 0;

void ISRcountFrontLeft(){
  frontLeftCounter++; 
}

void ISRtimerone(){
  Timer1.detachInterrupt(); // Stop the timer
  //  Serial.print("Motor Speed front left: ");
  float frontLeftRotation = (frontLeftCounter/diskslots)*60.00;
  //Serial.print(24.00/frontLeftCounter*119.4); Serial.print("mm, "); //24/(pi*38)
  Serial.print(frontLeftCounter*0.201); Serial.print("mm, "); //24/(pi*38)
  Serial.println("");
  frontLeftCounter = 0;
  Timer1.attachInterrupt(ISRtimerone);
}

void encoderSetup(){
 Serial.println("Encoder setup");
 Timer1.initialize(1000000);
 attachInterrupt(digitalPinToInterrupt(frontLeftPin), ISRcountFrontLeft, RISING);
 Timer1.attachInterrupt(ISRtimerone);

}
