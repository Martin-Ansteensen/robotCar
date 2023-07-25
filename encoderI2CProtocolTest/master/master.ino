#include <Wire.h>

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

void setup()
{
  Serial.println("Encoder initializing");
  Wire.begin();        
  Serial.begin(115200);  
  Serial.println(F("Master Ardunio Running"));
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
  
}

void loop()
{
  readEncoders();
  Serial.print("LEFT: "); Serial.print(left_encoder); Serial.print(" , "); Serial.print(left_change);
  Serial.print(" | RIGHT: "); Serial.print(right_encoder); Serial.print(" , "); Serial.print(right_change);
  Serial.print(" | NORMAL: "); Serial.print(normal_encoder); Serial.print(" , "); Serial.print(normal_change);
  Serial.println();
  delay(1000);

}
