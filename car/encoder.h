#ifndef _EncoderUnit_h
#define _EncoderUnit_h
#include <Wire.h>

# define I2C_LEFT_ENCODER_ADRESS 10
# define I2C_RIGHT_ENCODER_ADRESS 11
# define I2C_NORMAL_ENCODER_ADRESS 12


class EncoderUnit {
  private:
    int I2Cadress;
    int encoderCount;
    int encoderChange;
    int dirMultiplier;
    volatile byte lowerByteCount, upperByteCount, lowerByteChange, upperByteChange;

  public:
    EncoderUnit(int I2Cadress, int directionMultiplier = 1) {
      this->I2Cadress = I2Cadress;
      this->dirMultiplier = directionMultiplier;
    }

    void init(){
      Serial.print(I2Cadress); Serial.println(F(" running"));
      Wire.begin();
    }

    void requestEncoderData() {
      Wire.requestFrom(I2Cadress, 4); 
      upperByteCount = Wire.read();  
      lowerByteCount = Wire.read(); 
      encoderCount = (int)upperByteCount << 8 | (int)lowerByteCount; 
      encoderCount = encoderCount*dirMultiplier;
       
      upperByteChange = Wire.read();
      lowerByteChange = Wire.read(); 
      encoderChange = (int)upperByteChange << 8 | (int)lowerByteChange;
      encoderChange = encoderChange*dirMultiplier;
    }

    int getEncoderChange() {
      return encoderChange;
    }

    int getEncoderCount() {
      return encoderCount;
    }
};
#endif
