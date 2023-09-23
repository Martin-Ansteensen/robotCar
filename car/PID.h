#ifndef _PID_h
#define _PID_h
#include "Timer.h"

class PID{
    private:
        float kp, ki, kd;
        float previousError;
        float P, I, D;
        float correction;
        float dt;
        float error;
        Timer timer;
    public:
        PID(float kp, float ki, float kd){
            this->kp = kp;
            this->ki = ki;
            this->kd = kd;
            init();
        }

        void init(){
            previousError = 0;
            P = 0;
            I = 0;
            D = 0;
            correction = 0;
            Timer timer(MICROS);
            timer.start();
        }

        float calculateCorrection(float error){
            dt = timer.read()*1000;
            // Serial.println(dt);
            dt = 1;
            P = error*kp;
            I += ki*error*dt;
            D = kd*(error-previousError)/dt;
            correction = P + I + D;
            // Restart timer
            timer.pause();
            timer.start();
            previousError = error;
            return correction;
        }

        float getCorrection(){
            return correction;
        }

        float getError(){
            return error;
        }

};

#endif