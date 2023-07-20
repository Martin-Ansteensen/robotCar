#include "I2Cdev.h"
#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
unsigned long timer = 0;


float robotAngelScale180; // Goes from -PI, 0, PI (in degrees: -180, 0, 180)
float robotAngelScale360; // Goes from 0, 2PI (in degrees: 0, 360)
float gyro_curr = 0;
float gyro_prev = 0;

void gyroSetup() {
  Serial.println("Gyro running");
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
  delay(1000);
  Serial.println("Gyro-setup finished");
}


// Return rotation (only one angel)
void getRotation(){
   // mpu.update();
   // gyro_curr = mpu.getAngleZ();
    //gyro_curr = globalOrientation*57.2957795131;
    //robotAngelScale360 += gyro_prev - gyro_curr; // Consider Convert to rad
    robotAngelScale360 = globalOrientation*57.2957795131;
    if (robotAngelScale360 > 360){
      robotAngelScale360 = robotAngelScale360 - 360;
    } else if (robotAngelScale360 < -360){
      robotAngelScale360 = robotAngelScale360 + 360;
    }
    if (robotAngelScale360 > 180){
      robotAngelScale180 = robotAngelScale360 - 360;
    } else if (robotAngelScale360 < 180 and robotAngelScale360 > 0 ){
       robotAngelScale180 = robotAngelScale360;
    } else if (robotAngelScale360 < 0 and robotAngelScale360 > -180 ){
      robotAngelScale180 = robotAngelScale360;
    } else if (robotAngelScale360 < -180 ){
      robotAngelScale180 = robotAngelScale360 + 360;
    }

    //gyro_prev = gyro_curr;
    
    // Old code
    //Serial.println(robotAngelScale180);
    //robotAngelScale180 = robotAngelScale180*(1000/57296);
    //Serial.println(robotAngelScale180*57.2957);

}


float gyroMapping(float angle){
  if (angle >= 0 and angle <= M_PI){
    return angle;
  } else {
    return float(angle * (-1*M_PI) / (-2*M_PI) + 2*M_PI); // I have written out the map function to return floats
    // Look up arduino docs if problems
    
  }
}
