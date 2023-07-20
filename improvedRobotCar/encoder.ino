#include <Wire.h>
#include <LiquidCrystal_I2C.h> //Download & include the code library can be downloaded below

# define I2C_LEFT_ENCODER_ADRESS 10
# define I2C_RIGHT_ENCODER_ADRESS 11
# define I2C_NORMAL_ENCODER_ADRESS 12


// High and low bytes for count and change of encoders (left, right, normal)
volatile byte l1, l2, l1c, l2c;
volatile byte r1, r2, r1c, r2c;
volatile byte n1, n2, n1c, n2c;

int left_encoder = 0;
int left_change = 0;

int right_encoder = 0;
int right_change = 0;

int normal_encoder = 0;
int normal_change = 0;

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE); // Initialize LCD Display at address 0x27 


void encoderSetup()
{
  Serial.println("Encoder initializing");
  Wire.begin();      
  lcd.begin(16,2);
  lcd.setBacklight(HIGH); //Set Back light turn On  
  Serial.println("Encoder setup finished");
 
}

void readEncoders(){
  Wire.requestFrom(I2C_LEFT_ENCODER_ADRESS, 4); 
  l1 = Wire.read();  // Upper byte of encoder count
  l2 = Wire.read();  // Lower byte of encoder count
  left_encoder = (int)l1 << 8 | (int)l2;  
  l1c = Wire.read();  // Upper byte of encoder change
  l2c = Wire.read();  // Lower byte of encoder change
  left_change = (int)l1c << 8 | (int)l2c;  

  Wire.requestFrom(I2C_RIGHT_ENCODER_ADRESS, 4); 
  r1 = Wire.read();  // Upper byte of encoder count
  r2 = Wire.read();  // Lower byte of encoder count
  right_encoder = (int)r1 << 8 | (int)r2;
  r1c = Wire.read();  // Upper byte of encoder change
  r2c = Wire.read();  // Lower byte of encoder change
  right_change = (int)r1c << 8 | (int)r2c;  
  // Flip sign of right readings
  right_encoder = -1*right_encoder;
  right_change = -1*right_change;

  Wire.requestFrom(I2C_NORMAL_ENCODER_ADRESS, 4); 
  n1 = Wire.read();  // Upper byte of encoder count
  n2 = Wire.read();  // Lower byte of encoder count
  normal_encoder = (int)n1 << 8 | (int)n2;  
  n1c = Wire.read();  // Upper byte of encoder change
  n2c = Wire.read();  // Lower byte of encoder change
  normal_change = (int)n1c << 8 | (int)n2c;  

//  Serial.print("LEFT: "); Serial.print(left_encoder); Serial.print(" , "); Serial.print(left_change);
//  Serial.print(" | RIGHT: "); Serial.print(right_encoder); Serial.print(" , "); Serial.print(right_change);
//  Serial.print(" | NORMAL: "); Serial.print(normal_encoder); Serial.print(" , "); Serial.print(normal_change);
//  Serial.println();
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

// https://www.youtube.com/watch?v=ixsxDn_ddLE

// Constants for robot/wheelbase/odomtery configurarion

const double factor = 1000000;
const double cmPerTick = 0.59690260418*factor; // 2pi*3.8/40
const double encoderDist = 17.28*factor; // Distance from left- to right encoder
const double normalCmPerRadOffset = 14.4181646046*factor; // (11.92*2*3.8)/(2*pi)


double orientationChange = 0;
double horizontalChange = 0;
double centerChange = 0;

double dx, dy, d0;

double globalOrientation = 0;
double globalXPos = 0;
double globalYPos = 0;

void processEncoderData(){

  // Get current position
  dx = ((right_change + left_change)*cmPerTick)/2.0;  // Calculate the average change, accordingly the center change
  d0 = ((left_change-right_change)*cmPerTick)/encoderDist; // lenght of arc = angle/360deg * 2pi*r = angle/2pi * 2pi*r = angle*r (rad)
  dy = normal_change*cmPerTick - (d0*11.92*factor); //(orientationChange*normalCmPerRadOffset);

  // Calculate angle
  globalOrientation = globalOrientation + d0;

  // Get x and y component of the motion
//  globalXPos += (centerChange*sin(globalOrientation))/factor + (horizontalChange*cos(globalOrientation))/factor; // added  + orientationChange/2
//  globalYPos += (centerChange*cos(globalOrientation))/factor + (horizontalChange*sin(globalOrientation))/factor; //endret fra - til +. 

  globalXPos += -(dx*cos(globalOrientation))/factor + (dy*sin(globalOrientation))/factor; // added  + orientationChange/2
  globalYPos += (dy*cos(globalOrientation))/factor - (dx*sin(globalOrientation))/factor; //endret fra - til +. 




  
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


}
