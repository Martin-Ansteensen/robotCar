#include <Wire.h>
#include <LiquidCrystal_I2C.h> //Download & include the code library can be downloaded below

#define I2C_LEFT_ENCODER_ADRESS 10
#define I2C_RIGHT_ENCODER_ADRESS 11
#define I2C_NORMAL_ENCODER_ADRESS 12
double left_encoder = 0;
double right_encoder = 0;
double normal_encoder = 0;

int right_change = 0;
int left_change = 0;
int normal_change = 0;

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE); // Initialize LCD Display at address 0x27 

void encoderSetup() {
  Serial.println("Encoder initializing");
  Wire.begin();  
  lcd.begin(16,2);
  lcd.setBacklight(HIGH); //Set Back light turn On
  Serial.println("Encoder setup finished");
}

void readEncoders(){
  // Left encoder
  Wire.requestFrom(I2C_LEFT_ENCODER_ADRESS, 4);   
  left_change = Wire.read(); 
  if (left_change > 127){
    left_change = 256 - left_change;
    left_change *= -1;
  }
  left_encoder = left_encoder + left_change;
  /*Serial.print(F("Left : "));
  Serial.print(left_change);  
  Serial.print(", ");
  Serial.print(left_encoder);*/
  
  // Right encoder
  Wire.requestFrom(I2C_RIGHT_ENCODER_ADRESS, 4);   
  right_change = Wire.read(); 
  if (right_change > 127){
    right_change = 256 - right_change;
    right_change *= -1;
  }
  right_encoder = right_encoder - right_change;
  /*Serial.print(F(" RIGHT : "));
  Serial.print(right_change);  
  Serial.print(", ");
  Serial.print(right_encoder);*/

  // Normal encoder
  Wire.requestFrom(I2C_NORMAL_ENCODER_ADRESS, 4);   
  normal_change = Wire.read(); 
  if (normal_change > 127){
    normal_change = 256 - normal_change;
    normal_change *= -1;
  }
  normal_encoder = normal_encoder - normal_change;
  /*Serial.print(F(" Normal : "));
  Serial.print(normal_change);  
  Serial.print(", ");
  Serial.println(normal_encoder);*/
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
const double cmPerTick = 0.59690260418*factor; // pi*3.8/40
const double encoderDist = 17.28*factor; // Distance from left- to right encoder
const double normalCmPerRadOffset = 14.4181646046*factor; // (11.92*2*3.8)/(2*pi)


int prevleft_encoder = 0;
int prevright_encoder = 0;
int prevnormal_encoder = 0;

int leftChange = 0;
int rightChange = 0;
int rawHorizontalChange = 0;
double orientationChange = 0;
double horizontalChange = 0;
double centerChange = 0;

double globalOrientation = 0;
double globalXPos = 0;
double globalYPos = 0;

void processEncoderData(){

  // Get current position
  leftChange = (left_encoder - prevleft_encoder);
  rightChange = (right_encoder - prevright_encoder);
  rawHorizontalChange = (normal_encoder - prevnormal_encoder);
  centerChange = ((rightChange + leftChange)*cmPerTick)/2.0;  // Calculate the average change, accordingly the center change
  
  // Calculate angle
  orientationChange = ((leftChange-rightChange)*cmPerTick)/encoderDist; // lenght of arc = angle/360deg * 2pi*r = angle/2pi * 2pi*r = angle*r (rad)
  globalOrientation = globalOrientation + orientationChange;

  // Get x and y component of the motion
  horizontalChange = rawHorizontalChange*cmPerTick + (orientationChange*normalCmPerRadOffset);
  globalXPos += (centerChange*sin(globalOrientation))/factor + (horizontalChange*cos(globalOrientation))/factor; // added  + orientationChange/2
  globalYPos += (centerChange*cos(globalOrientation))/factor + (horizontalChange*sin(globalOrientation))/factor; //endret fra - til +. 
  
  // Print position data to console
  /*Serial.print("x, y and angle traveled in cm: "); 
  Serial.print(globalXPos); Serial.print(", "); 
  Serial.print(globalYPos); Serial.print(", "); 
  Serial.print(globalOrientation*57.2957795131);
  Serial.println();*/
  lcd.setCursor(0,0); // Move cursor to 0
  lcd.print("x"); 
  lcd.setCursor(1,0);
  lcd.print(globalXPos);
  
  lcd.setCursor(8,0); // Move cursor to 0
  lcd.print("y"); 
  lcd.setCursor(9,0);
  lcd.print(globalYPos);

  lcd.setCursor(0,1); // Move cursor to 0
  lcd.print("rot: "); 
  lcd.setCursor(5,1);
  lcd.print(globalOrientation*57.2957795131);


  // Update variables holding previous values
  prevleft_encoder = left_encoder;
  prevright_encoder = right_encoder;
  prevnormal_encoder = normal_encoder;
}

 
