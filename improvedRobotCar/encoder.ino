
#define inputClkVerticalLeftEncoder 46
#define inputClkVerticalRightEncoder 44
#define inputClkNormalEncoder 48

#define inputDirVerticalLeftEncoder 47
#define inputDirVerticalRightEncoder 45
#define inputDirNormalEncoder 49

// The sublists are the encoders in this order: verticalLeft, verticalRight, normal
// The content of these sublists are: [inputClock, inputDirection, counter, currentClockState, prevClockState]
int encoderData[][5] = {
  {inputClkVerticalLeftEncoder, inputDirVerticalLeftEncoder, 0, 0, 0}, 
  {inputClkVerticalRightEncoder, inputDirVerticalRightEncoder, 0, 0, 0}, 
  {inputClkNormalEncoder, inputDirNormalEncoder, 0, 0, 0}
};


void encoderSetup() {
  Serial.println("Encoder initializing");
  //  Set the encoder pins as inputs
  pinMode(encoderData[0][0], INPUT);
  pinMode(encoderData[1][0], INPUT);
  pinMode(encoderData[2][0], INPUT);

  pinMode(encoderData[0][1], INPUT);
  pinMode(encoderData[1][1], INPUT);
  pinMode(encoderData[2][1], INPUT);

  // Read the initial state of the encoder
  encoderData[0][4] = digitalRead(encoderData[0][0]);
  encoderData[1][4] = digitalRead(encoderData[1][0]);
  encoderData[2][4] = digitalRead(encoderData[2][0]);
  Serial.println("Encoder setup finished");
}


void readEncoders(){

  for (int i = 0; i < 3; i++) {
    // Read the current state of the encoder
    encoderData[i][3] = digitalRead(encoderData[i][0]);
    
    // If the previous and the current state of the inputCLK are different then a pulse has occured
    if (encoderData[i][3] != encoderData[i][4]){
      
      // If the direction state is different than the clock state then 
      // the encoder is rotating counterclockwise
      if (digitalRead(encoderData[i][1]) != encoderData[i][4]){

        if (i == 0){
          encoderData[i][2] ++;  //Invert the left encoder
        } else{
          encoderData[i][2] --;  
        }
        
      // The direction state is the same as the clock state
      // the encoder is rotating clockwise
      } else {
        
        if (i == 0){
          encoderData[i][2] --;  
        } else{
          encoderData[i][2] ++;  
        }
        
      
      }
    }
    encoderData[i][4] = encoderData[i][3];
  }
  
/*
  Serial.print("Left, right and bottom encoder: "); 
  Serial.print(encoderData[0][2]); Serial.print(", "); //0.29845130209
  Serial.print(encoderData[1][2]); Serial.print(", "); 
  Serial.print(encoderData[2][2]);
  Serial.println();
*/
}

int factor = 10000;

double cmPerTick = 0.2984*factor; //0.29845130209 = pi*3.8/40 (pi*wheel diameter/ticks per rev)
int verticalLeftEncoder;
int prevVerticalLeftEncoder = 0;
int verticalRightEncoder;
int prevVerticalRightEncoder = 0;
int normalEncoder;
int prevNormalEncoder = 0;

double leftChange;
double rightChange;
double changeOrientation;
double orientationRads;

int rawHorizontalChange;
double horizontalChange;

// The normal turns 60ticks per 90deg

// 17 cm per 90deg
// 10.8 = 17/(pi/2) (90deg)

// 11.92 cm from center to wheel center
// Calculate cm per rad: (11.92*2*3.8)/(pi*2) = 14.4181646046
double normalCmPerRadOffset = 14.4181*factor;

// (24*pi)/(pi/38)= num rotations per 360deg
// 1.57*40 = 63 ticks per 360 deg

double centerChange = 0;
double globalXPos = 0;
double globalYPos = 0;
double encoderDist = 17.28*factor;

double tmp1 = 0;

void processEncoderData(){
  verticalLeftEncoder = encoderData[0][2]; 
  verticalRightEncoder = encoderData[1][2];
  normalEncoder = encoderData[2][2];

  // Get current position
  leftChange = (verticalLeftEncoder - prevVerticalLeftEncoder)*cmPerTick;
  rightChange = (verticalRightEncoder - prevVerticalRightEncoder)*cmPerTick;
  rawHorizontalChange = (normalEncoder - prevNormalEncoder)*cmPerTick;

  // Calculate angle
  changeOrientation = (leftChange-rightChange)/encoderDist; // arcLength = angle/360deg * 2pi*r = angle/2pi * 2pi*r = angle*r
  orientationRads = orientationRads + changeOrientation;

  // Get x and y component of the motion
  horizontalChange = rawHorizontalChange + (changeOrientation*normalCmPerRadOffset);
  centerChange = (rightChange + leftChange)/2;        // Creates degrees
  globalXPos += (centerChange*sin(orientationRads))/factor + (horizontalChange*cos(orientationRads))/factor;// + (changeOrientation*normalCmPerRadOffset/factor*cmPerTick/factor) ; // switch cos and sin for x and y first component
  globalYPos += (centerChange*cos(orientationRads))/factor - (horizontalChange*sin(orientationRads))/factor;

//  globalXPos += (centerChange*sin(orientationRads))/factor + (horizontalChange*cos(orientationRads))/factor  ;
//  globalYPos += (centerChange*cos(orientationRads))/factor - (horizontalChange*sin(orientationRads))/factor  ;


  Serial.print("x, y and angle traveled in cm: "); 
  Serial.print(globalXPos); Serial.print(", "); 
  Serial.print(globalYPos); Serial.print(", "); 
  Serial.print(orientationRads*57.3);

/*
  if (true){
    Serial.print("angle, raw x: "); 
    Serial.print(orientationRads*57.3); Serial.print(", "); 
    tmp1  += (normalEncoder - prevNormalEncoder)*0.2984; 
    Serial.print(tmp1); Serial.print(", ");    
    Serial.println();
    
   }*/
   Serial.println();

  prevVerticalLeftEncoder = verticalLeftEncoder;
  prevVerticalRightEncoder = verticalRightEncoder;
  prevNormalEncoder = normalEncoder;

}

 
