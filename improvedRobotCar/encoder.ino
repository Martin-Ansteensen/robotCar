
#define inputClkVerticalLeftEncoder 46
#define inputClkVerticalRightEncoder 44
#define inputClkNormalEncoder 48

/*#define inputDirVerticalLeftEncoder 47
#define inputDirVerticalRightEncoder 45
#define inputDirNormalEncoder 49*/

#define inputDirVerticalLeftEncoder 19
#define inputDirVerticalRightEncoder 18
#define inputDirNormalEncoder 20


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
  
  // Attach a interrupt to the dir pin og each encoder
  attachInterrupt(digitalPinToInterrupt(inputDirVerticalLeftEncoder), readLeft, HIGH);
  attachInterrupt(digitalPinToInterrupt(inputDirVerticalRightEncoder), readRight, HIGH);
  attachInterrupt(digitalPinToInterrupt(inputDirNormalEncoder), readNormal, HIGH);


  Serial.println("Encoder setup finished");
}

void readLeft(){
 readEncoders(0);
}

void readRight(){
  readEncoders(1);
}

void readNormal(){
  readEncoders(2);
}

void readEncoders(int pin){
  // Read the current state of the encoder
  encoderData[pin][3] = digitalRead(encoderData[pin][0]);
  
  // If the previous and the current state of the inputCLK are different then a pulse has occured
  if (encoderData[pin][3] != encoderData[pin][4]){
    
    // If the direction state is different than the clock state then 
    // the encoder is rotating counterclockwise
    if (digitalRead(encoderData[pin][1]) != encoderData[pin][4]){

      if (pin == 0){  // Invert the left encoder compared to the left and right
        encoderData[pin][2] ++;  
      } else{
        encoderData[pin][2] --;  
      }
      
    // The direction state is the same as the clock state
    // the encoder is rotating clockwise
    } else {
      
      if (pin == 0){  // Invert the left encoder compared to the left and right
        encoderData[pin][2] --;  
      } else{
        encoderData[pin][2] ++;  
      }
      
    
    }
  }
  encoderData[pin][4] = encoderData[pin][3]; // Set prevClockState equal to current clock state
  
/*
  Serial.print("Left, right and bottom encoder: "); 
  Serial.print(encoderData[0][2]); Serial.print(", "); //0.29845130209
  Serial.print(encoderData[1][2]); Serial.print(", "); 
  Serial.print(encoderData[2][2]);
  Serial.println();
*/
}

// Research why one get strange results with big int*double


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


int verticalLeftEncoder;
int prevVerticalLeftEncoder = 0;
int verticalRightEncoder;
int prevVerticalRightEncoder = 0;
int normalEncoder;
int prevNormalEncoder = 0;

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
  verticalLeftEncoder = encoderData[0][2]; 
  verticalRightEncoder = encoderData[1][2];
  normalEncoder = encoderData[2][2];

  // Get current position
  leftChange = (verticalLeftEncoder - prevVerticalLeftEncoder);
  rightChange = (verticalRightEncoder - prevVerticalRightEncoder);
  rawHorizontalChange = (normalEncoder - prevNormalEncoder);
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
  Serial.print(verticalLeftEncoder); Serial.print(", "); 
  Serial.print(verticalRightEncoder); Serial.print(", "); 
  Serial.print(normalEncoder);
  Serial.println();

  // Update variables holding previous values
  prevVerticalLeftEncoder = verticalLeftEncoder;
  prevVerticalRightEncoder = verticalRightEncoder;
  prevNormalEncoder = normalEncoder;
}

 
