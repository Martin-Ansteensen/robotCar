#ifndef _Reciever_h
#define _Reciever_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

class Reciever{
    private:
        unsigned long lastReceiveTime=0, currentTime=0;
        double  driveJoystickX=0,  driveJoystickY=0, rotationJoystickX=0, rotationJoystickY=0;

        // Create a variable with the above structure
        byte data[12]; 

        
        bool firstRecieve = true;

        int driveX, driveY, drive0, rotation0;
        float driveMagnitude, rotationChange;
        int driveXoffset=0, driveYoffset=0, rotation0offset=0;

        RF24& radio;

    public:
        Reciever(RF24& _radio) : radio(_radio){}

        void init(){
            Serial.println("Reciever setup");
            radio.begin();
            const byte address[6] = "00001";
            radio.openReadingPipe(0, address);
            radio.setAutoAck(false);
            // radio.setDataRate(RF24_250KBPS);
            radio.setPALevel(RF24_PA_MIN);
            radio.startListening(); //  Set the module as receiver
            Serial.println("Reciever setup finished");
        }

        void recieveData(){
            // Check whether there is data to be received or not
            if (radio.available()) {
                // Read the whole data and store it into the 'data' structure
                radio.read(&data, sizeof(data)); 
                // At this moment we have received the data
                lastReceiveTime = millis();
                // If this was the first time we recieved values from the transmitter
                // we store them in
                if (firstRecieve){
                    firstRecieve = false;
                    processData();
                    driveXoffset = driveX;
                    driveYoffset = driveY;
                    rotation0offset = rotation0;
                }
            }
            // Check whether we keep receving data, or we have a connection between the two modules
            currentTime = millis();
            // The connection is lost if more than 1 second
            // has passed since the last time we recieved data
            if (currentTime - lastReceiveTime > 1000) {
                resetData();
            }
        }        

        void resetData(){
            driveMagnitude = 0;
        }

        void processData(){
            driveX = map(data[0], 0, 255, -123, 122) - driveXoffset;
            driveY = map(data[1], 0, 255, -123, 122) - driveYoffset;
            drive0 = -1*atan2(driveX, driveY) - (M_PI / 2);
            driveMagnitude = abs(max(abs(driveX), abs(driveY))); //sqrt(pow(driveX, 2) + pow(driveY, 2))/1.41;
            rotation0 = map(data[3], 0, 255, -123, 122) - rotation0offset;
            if (rotation0 == 0){
              rotationChange = 0;
            } else {
             rotationChange = 0.1*rotation0/abs(rotation0);
            }
            
//            Serial.print(driveX); Serial.print(" , ");
//            Serial.print(driveY); Serial.print(" , ");
//            Serial.print(rotation0); Serial.print(" | ");
//
//
//            Serial.print(driveXoffset); Serial.print(" , ");
//            Serial.print(driveYoffset); Serial.print(" , ");
//            Serial.print(rotation0offset); Serial.println();


        }

        float getRotationChange(){
            return rotationChange;
        }
        float getDriveMagnitude(){
            return driveMagnitude;
        }

        int getDrive0(){
          return drive0;
        }

};

#endif
