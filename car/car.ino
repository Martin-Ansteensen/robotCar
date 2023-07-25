#include <LiquidCrystal_I2C.h>
#include <MPU6050_light.h>
#include "Wire.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#include "encoder.h"
#include "navigation.h"
#include "motor.h"
#include "drivetrain.h"
#include "reciever.h"

//  Connections from L298N to Arduino Mega

// Front left
#define enBLeft 7
#define in3Left 24
#define in4Left 22

//  Back left
#define enALeft 6
#define in1Left 28
#define in2Left 26


//  Front right
#define enARight 5
#define in1Right 25
#define in2Right 23

//  Back right
#define enBRight 4
#define in3Right 29
#define in4Right 27



unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds

int leftChange, rightChange, normalChange;
double x = 0, y = 0, rotation = 0;
float error = 0;

float angle = 0;
int n = 0;
int mspeed = 125;
double inspeed, inangle;


LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Initialize LCD Display at address 0x27

EncoderUnit leftEncoder(I2C_LEFT_ENCODER_ADRESS);
EncoderUnit rightEncoder(I2C_RIGHT_ENCODER_ADRESS, -1);
EncoderUnit normalEncoder(I2C_NORMAL_ENCODER_ADRESS);

MPU6050 mpu(Wire);
Navigation navigate(mpu);

Motor frontLeftMotor(enBLeft, in3Left, in4Left, true, 0);
Motor frontRightMotor(enARight, in1Right, in2Right, false, 0);
Motor backLeftMotor(enALeft, in1Left, in2Left, false, 7);
Motor backRightMotor(enBRight, in3Right, in4Right, true, 15);

DriveTrain driveTrain(frontLeftMotor, frontRightMotor, backLeftMotor, backRightMotor, 50, 0.40, 30);

RF24 radio(49, 48);  // nRF24L01 (CE, CSN)
Reciever reciever(radio);


void setup() {
  Serial.begin(9600);
  Serial.println("Robotcar initializing");

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH); //Set Back light turn On

  leftEncoder.init();
  rightEncoder.init();
  normalEncoder.init();
  navigate.init();
  reciever.init();
  Serial.println("Robotcar finished initializing");
  startMillis = millis();  //initial start time


}

void displayPosition(double x, double y, double rotation) {
  // Top left corner
  lcd.setCursor(0, 0);
  lcd.print("x");
  lcd.setCursor(1, 0);
  lcd.print(x);
  // Top middel screen
  lcd.setCursor(8, 0);
  lcd.print("y");
  lcd.setCursor(9, 0);
  lcd.print(y);
  // Bottom left corner
  lcd.setCursor(0, 1);
  lcd.print("rot: ");
  lcd.setCursor(5, 1);
  lcd.print(rotation * 57.2957795131);
}

void loop() {
  if (n < 60) {
    mspeed = 80;
//    Serial.println("forward");
  }

  else {
    mspeed = -80;
//    Serial.println("backward");
  }
  if (n > 120) {
    n = 0;
  }
  n++;

  leftEncoder.requestEncoderData();
  rightEncoder.requestEncoderData();
  normalEncoder.requestEncoderData();

  leftChange = leftEncoder.getEncoderChange();
  rightChange = rightEncoder.getEncoderChange();
  normalChange = normalEncoder.getEncoderChange();

  reciever.recieveData();
  reciever.processData();
  
  navigate.calculateNewPosition(leftChange, rightChange, normalChange);
  x = navigate.getXpos();
  y = navigate.getYpos();
  rotation = navigate.getOdometryOrientation();

  displayPosition(x, y, rotation);
  angle += reciever.getRotationChange();

  navigate.setOrientationTarget(angle);
  navigate.calculateOrientationError();
  error = navigate.getOrientationError();
  driveTrain.driveWithPID(reciever.getDriveMagnitude(), reciever.getDrive0(), error);
  Serial.print(error); Serial.print(", ");Serial.println(driveTrain.getCorrection());


}
