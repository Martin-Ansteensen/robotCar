#ifndef _Navigation_h
#define _Navigation_h

#include "I2Cdev.h"
#include "Wire.h"
#include <MPU6050_light.h>


class Navigation{
  private:
    // All lengths are in cm
    // All angles are in radians
    // Omni (Odomoetry) wheel radius = 3.8cm
    // Meccanum wheel diameter = 8cm
    // Distance between left and right encoder = 17.28cm
    // Distance from normal encoder to center of robot = 11.92cm
    // Encoder ticks per revolution = 40

    double factor = 1000000;
    double sideEncoderDist = 17.28*factor; // Distance from left- to right encoder
    double normalEncoderDist = 11.92*factor;
    double cmPerTick = 0.59690260418*factor; // 2pi*3.8/40

    double dx=0, dy=0, d0=0; // Change in x, y and orientation

    // Clockwise is positive rotation
    double odometryOrientation = 0;
    double xPos = 0;
    double yPos = 0;

    double gyroOrientation=0;

    // Target between -pi and pi
    float orientationTarget=0, orientationError=0;

    MPU6050& mpu;

  public:

    Navigation(MPU6050& _mpu): mpu(_mpu) {}

    void init(){
      
        // // Initialize MPU6050
        // MPU6050 mpu(Wire);
        // Wire.begin();
        // while(mpu.begin() != 0){ 
        // Serial.println(F("Could not find a valid MPU6050 sensor, check wiring!"));
        // delay(500); 
        // }
        // Serial.println(F("Calculating offsets, do not move MPU6050"));
        // delay(1000);
        // mpu.calcOffsets(); // gyro and accelero
        // Serial.println("MPU6050-setup finished");
    }

    void calculateNewPosition(int dL, int dR, int dN){ // Change at left-, right and normal encoder
      /* Calculates new position given small change 
      in encoder values*/
      
      // Get change in position
      dx = ((dR + dL)*cmPerTick)/2.0;  // Calculate the average change, accordingly the center change
      d0 = ((dL-dR)*cmPerTick)/sideEncoderDist; // lenght of arc l = angle*r (rad)
      dy = dN*cmPerTick - (d0*normalEncoderDist);

      // Calculate new orientation and position
      odometryOrientation += d0;
      xPos += (dx*cos(odometryOrientation))/factor + (dy*sin(odometryOrientation))/factor; 
      yPos += (dy*cos(odometryOrientation))/factor - (dx*sin(odometryOrientation))/factor; 

    }

    void updateGyro(){
      mpu.update();
      gyroOrientation = mpu.getAngleZ();
      Serial.println(gyroOrientation);
    }

    void calculateOrientationError(){
      // One can use the orientation provided by the gyro
      // or the odometry
      double orientation = odometryOrientation;
      // Reduce orientation to lie between between -pi and pi
      while (orientation < -M_PI){
        orientation += M_PI;
      }
      while (orientation > M_PI){
        orientation -= M_PI;
      }
      orientationError = orientationTarget - orientation;

    }

    double getOrientationError(){
      return orientationError;
    }

    void setOrientationTarget(float target){
      orientationTarget = target;
    }

    double getGyroOrientation(){
      return mpu.getAngleZ();
    }

    double getXpos(){
      return xPos;
    }
    
    double getYpos(){
      return yPos;
    }

    double getOdometryOrientation(){
      return odometryOrientation;
    }
};
#endif
