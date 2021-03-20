#include <Wire.h>

# define I2C_LEFT_ENCODER_ADRESS 11
# define I2C_RIGHT_ENCODER_ADRESS 12
double left_encoder = 0;
double right_encoder = 0;
double normal_encoder = 0;

int right_change = 0;
int left_change = 0;
int normal_change = 0;

void setup()
{
  Wire.begin();        
  Serial.begin(9600);  

  Serial.println(F("-------------------------------------I am the Master"));
  delay(1000);
 
}


void loop()
{
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
  Serial.println(right_encoder); 
  delay(500);

}
