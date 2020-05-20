#include "TimerOne.h"

// Speed sensors ports
const byte speedEnAL = 18;
const byte speedEnAR = 19;
const byte speedEnBL = 20;
const byte speedEnBR = 21;

int diskslots = 20; // Number of slots in speedmeasuring disk

// Counters for the rotary encoder
unsigned int counter1 = 0;
unsigned int counter2 = 0;
unsigned int counter3 = 0;
unsigned int counter4 = 0;

void ISRcount1(){
  counter1++;
}

void ISRcount2(){
  counter2++;
}

void ISRcount3(){
  counter3++;
}

void ISRcount4(){
  counter4++;
}

void ISRtimerone(){
  Timer1.detachInterrupt(); // Stop the timer
  
//  Serial.print("Motor Speed front left: ");
  float rotation1 = (counter1/diskslots)*60.00;
  Serial.println(rotation1);
//  
//  Serial.print("Motor Speed back left: ");
  float rotation2 = (counter2/diskslots)*60.00;
  Serial.println(rotation2);
//  
//  Serial.print("Motor Speed front right: ");
  float rotation3 = (counter3/diskslots)*60.00;
  Serial.println(rotation3);
//  
//  Serial.print("Motor Speed back right: ");
  float rotation4 = (counter4/diskslots)*60.00;
  Serial.println(rotation4);
  Serial.println("");

  counter1 = 0;
  counter2 = 0;
  counter3 = 0;
  counter4 = 0;
 Timer1.attachInterrupt(ISRtimerone);
}

void encoderSetup(){
 Timer1.initialize(1000000);
 attachInterrupt(digitalPinToInterrupt(speedEnAL), ISRcount1, RISING);
 attachInterrupt(digitalPinToInterrupt(speedEnAR), ISRcount2, RISING);
 attachInterrupt(digitalPinToInterrupt(speedEnBL), ISRcount3, RISING);
 attachInterrupt(digitalPinToInterrupt(speedEnBR), ISRcount4, RISING);
 Timer1.attachInterrupt(ISRtimerone);

}
