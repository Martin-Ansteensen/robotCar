#include "RotaryEncoder.h"
#include <Wire.h>

// One of these should be uncommented
#define I2C_SLAVE_ADDRESS 10 // Left
//#define I2C_SLAVE_ADDRESS 11 // Right
//#define I2C_SLAVE_ADDRESS 12 // Normal


int enocoderCount = 0;
int lastEncoderCount = 0;
int encoderChange = 0;

RotaryEncoder Encoder(&EncoderChanged, 2, 3); // Pins 2 (DT), 3 (CLK)

void EncoderChanged()
{
  const unsigned int state = Encoder.GetState();
  
  if (state & DIR_CW)  
    enocoderCount++;
    
  if (state & DIR_CCW)  
    enocoderCount--;    
}


void setup()
{
  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(115200); 
  Serial.print("Arduino Pro Mini counting encoder ticks, adress: ");             
  Serial.println(I2C_SLAVE_ADDRESS);             
  Wire.onRequest(sendEncoderData);
  Encoder.setup();
}

void loop(){}


void sendEncoderData()
{ 
  // Calculate change since last request
  encoderChange = enocoderCount - lastEncoderCount;
  // Update last count
  lastEncoderCount = enocoderCount;
  Serial.print(F("Sending count: ")); Serial.print(enocoderCount); 
  Serial.print(F(" and change: : ")); Serial.print(encoderChange); 
  Wire.write(highByte(enocoderCount));
  Wire.write(lowByte(enocoderCount));
  Wire.write(highByte(encoderChange));
  Wire.write(lowByte(encoderChange));
  
}
