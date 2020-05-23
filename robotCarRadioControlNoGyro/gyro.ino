#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false; // set true if DMP init was successful
uint8_t mpuIntStatus; // holds actual interrupt status byte from MPU
uint8_t devStatus; // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize; // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount; // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q; // [w, x, y, z] quaternion container
VectorFloat gravity; // [x, y, z] gravity vector
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
float ypr[3]; // [yaw, pitch, roll] yaw/pitch/roll container and gravity vector

float rotation, roll, tilt, forwardAcceleration, sideAcceleration, fallAcceleration; 
float rotationOffset;

float rotation180, rotation360;

volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high

void dmpDataReady(){
  mpuInterrupt = true;
}

void gyroSetup() {
  Serial.println("Gyro running");
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
  #endif
  
  Serial.println("Initializing gyro sensor");
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  
  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(33);
  mpu.setYGyroOffset(-46);
  mpu.setZGyroOffset(16);
  mpu.setXAccelOffset(-3741); // 1688 factory default for my test chip
  mpu.setYAccelOffset(622); // 1688 factory default for my test chip
  mpu.setZAccelOffset(1815); // 1688 factory default for my test chip
  
  // make sure gyro is initialized
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);
    
    // enable Arduino interrupt detection
    attachInterrupt(digitalPinToInterrupt(2), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    
    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    dmpReady = true;
    
    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
  Serial.println("Waiting for gyrovalues to stabilize");
  getRotation();
  delay(10000);
  Serial.println("Gyro-setup finished");
}


// Return rotation (only one angel)
void getRotation(){
  if (!dmpReady) return;
  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize){
    //no mpu data - performing PID calculations and output to motors 
    //pid.Compute();
    //motorController.move(output, MIN_ABS_SPEED);
  }
  
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
  
  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024){
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  }
  else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
    
    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;
    
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    rotation360 = gyroMapping(ypr[0]*57.2957); // The last part converts to deg
    rotation180 = ypr[0] * 57.2957;
  }
}


float gyroMapping(float angle){
  if (angle >= 0 and angle <= 180){
    return angle;
  } else {
    return float((angle - 0) * (180 - 360) / (-180.0 -180.0) + 360); // I have written out the map function to return floats
    //return float((angle - 0) * (M_PI - 2*M_PI) / (-2*M_PI) + 2*M_PI); // I have written out the map function to return floats
  }
}
