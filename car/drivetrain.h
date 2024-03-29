#ifndef _DriveTrain_h
#define _DriveTrain_h
#include "motor.h"
#include "PID.h"

// https://arduino.stackexchange.com/questions/24033/proper-use-of-and-when-passing-objects-in-methods

class DriveTrain {
    private:
        // Pointers to motor-objects
        Motor& frontLeft, frontRight, backLeft, backRight;
        double motorVelocities[4];
        float rot_correction;
        PID& rotPID;

    public:
        DriveTrain(Motor& _frontLeft, Motor& _frontRight, Motor& _backLeft, Motor& _backRight, PID& _rotPID) 
                  : frontLeft(_frontLeft), frontRight(_frontRight), backLeft(_backLeft), backRight(_backRight), rotPID(_rotPID) {
            init();
        }

        void init(){
        }

        void caculateVelocities(int motorSpeed, float angle){
            motorVelocities[0] = (motorSpeed*sin(angle+M_PI/4)*sqrt(2)); // Front left
            motorVelocities[1] = (motorSpeed*cos(angle+M_PI/4)*sqrt(2)); // Front right
            motorVelocities[2] = (motorSpeed*cos(angle+M_PI/4)*sqrt(2)); // Back left
            motorVelocities[3] = (motorSpeed*sin(angle+M_PI/4)*sqrt(2)); // Back right
        }

        void drive(int motorSpeed, float angle){
            caculateVelocities(motorSpeed, angle);
            frontLeft.drive(motorVelocities[0]);
            frontRight.drive(motorVelocities[1]);
            backLeft.drive(motorVelocities[2]);
            backRight.drive(motorVelocities[3]);
        }


        void scaleVelocities(){
            // Scales velocities so that none of them exceeds
            // 255 in magnitude
            double largest = abs(motorVelocities[0]);
            for (int i=1; i<4; i++){
                if(abs(motorVelocities[i]) > largest){
                    largest = abs(motorVelocities[i]);
                }
            }
            if (largest < 255){return;}
            // Only scale if the largest velocity exceeds 255 in magnitude
            for (int i=0; i<4; i++){
                motorVelocities[i] *= 255.0/largest;
            }
        }

        void driveWithPID(int motorSpeed, float angle, float error){
            rot_correction = rotPID.calculateCorrection(error);
            caculateVelocities(motorSpeed, angle);
            // Add error adjustment to velocities
            motorVelocities[0] += rot_correction;
            motorVelocities[1] -= rot_correction;
            motorVelocities[2] += rot_correction;
            motorVelocities[3] -= rot_correction;

            scaleVelocities();

            // Set motor velocities
            frontLeft.drive(motorVelocities[0]);
            frontRight.drive(motorVelocities[1]);
            backLeft.drive(motorVelocities[2]);
            backRight.drive(motorVelocities[3]);
        }

        void stop(){
            frontLeft.disable();
            frontRight.disable();
            backLeft.disable();
            backRight.disable();
        }

        float getCorrection(){
          return rot_correction;
        }
};
#endif
