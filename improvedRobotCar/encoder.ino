
#include <Wire.h>

# define I2C_LEFT_ENCODER_ADRESS 10
# define I2C_RIGHT_ENCODER_ADRESS 11
# define I2C_NORMAL_ENCODER_ADRESS 12
double left_encoder = 0;
double right_encoder = 0;
double normal_encoder = 0;

int right_change = 0;
int left_change = 0;
int normal_change = 0;


void encoderSetup() {
  Serial.println("Encoder initializing");
  Wire.begin();  
  Serial.println("Encoder setup finished");
}

void readEncoders(){
  /*
  // Left encoder
  Wire.requestFrom(I2C_LEFT_ENCODER_ADRESS, 4);   
  left_change = Wire.read(); 
  Serial.print(F("LEFT : "));
  if (left_change > 127){
    left_change = 256 - left_change;
    left_change *= -1;
  }
  Serial.print(left_change);  
  Serial.print(", ");
  left_encoder = left_encoder + left_change;
  Serial.println(left_encoder);  

  // Right encoder
  Wire.requestFrom(I2C_RIGHT_ENCODER_ADRESS, 4);   
  right_change = Wire.read(); 
  Serial.print(F("RIGHT : "));
  if (right_change > 127){
    right_change = 256 - right_change;
    right_change *= -1;
  }
  Serial.print(right_change);  
  Serial.print(", ");
  right_encoder = right_encoder + right_change;
  Serial.print(right_encoder); */

  // Normal encoder
  Wire.requestFrom(I2C_NORMAL_ENCODER_ADRESS, 4);   
  normal_change = Wire.read(); 
  Serial.print(F("Normal : "));
  if (normal_change > 127){
    normal_change = 256 - normal_change;
    normal_change *= -1;
  }
  Serial.print(normal_change);  
  Serial.print(", ");
  normal_encoder = normal_encoder + normal_change;
  Serial.println(normal_encoder);
}


//
// ROBOT FACTS
// OMNI WHEEL DIAMETER: 3.8CM
// MECCANUM WHEEL DIAMETER: 8CM
// DISTACNE FROM LEFT- TO RIGHT ENCODER: 17.28 CM
// DISTANCE FROM NORMAL ENCODER TO ROBOT CENTER: 11.92CM
// ENCODER TICKS PER REVOLUTION: 40
// ALL ANGLES ARE IN RADS
//

// Constants for robot/wheelbase/odomtery configurarion

const double factor = 1000000;
const double cmPerTick = 0.29845130209*factor; // pi*3.8/40
const double encoderDist = 17.28*factor; // Distance from left- to right encoder
const double normalCmPerRadOffset = 14.4181646046*factor; // (11.92*2*3.8)/(2*pi)


int prevleft_encoder = 0;
int prevright_encoder = 0;
int prevnormal_encoder = 0;

int leftChange;
int rightChange;
int rawHorizontalChange;
double orientationChange;
double horizontalChange;
double centerChange;

double globalOrientation;
double globalXPos;
double globalYPos;

void processEncoderData(){

  // Get current position
  leftChange = (left_encoder - prevleft_encoder);
  rightChange = (right_encoder - prevright_encoder);
  rawHorizontalChange = (normal_encoder - prevnormal_encoder);
  centerChange = ((rightChange + leftChange)*cmPerTick)/2.0;  // Calculate the average change, accordingly the center change
  
  // Calculate angle
  orientationChange = ((leftChange-rightChange)*cmPerTick)/encoderDist; // lenght of arc = angle/360deg * 2pi*r = angle/2pi * 2pi*r = angle*r
  globalOrientation = globalOrientation + orientationChange;

  // Get x and y component of the motion
  horizontalChange = rawHorizontalChange*cmPerTick + (orientationChange*normalCmPerRadOffset);
  globalXPos += (centerChange*sin(globalOrientation))/factor + (horizontalChange*cos(globalOrientation))/factor;
  globalYPos += (centerChange*cos(globalOrientation))/factor - (horizontalChange*sin(globalOrientation))/factor;

  // Print position data to console
  Serial.print("x, y and angle traveled in cm: "); 
  Serial.print(globalXPos); Serial.print(", "); 
  Serial.print(globalYPos); Serial.print(", "); 
  Serial.print(globalOrientation);
  Serial.println();

  // Update variables holding previous values
  prevleft_encoder = left_encoder;
  prevright_encoder = right_encoder;
  prevnormal_encoder = normal_encoder;
}

 
