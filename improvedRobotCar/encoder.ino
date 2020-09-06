
#define inputClkVerticalLeftEncoder 1
#define inputClkVerticalRightEncoder 2
#define inputClkNormalEncoder 3

#define inputDirVerticalLeftEncoder 4
#define inputDirVerticalRightEncoder 5
#define inputDirNormalEncoder 6

// int verticalLeftEncoderCounter = 0;
// int verticalLeftEncoderCLK;
// int prevVerticalLeftEncoderCLK;

// int verticalRightEncoderCounter = 0;
// int verticalRightEncoderCLK;
// int prevVerticalRightEncoderCLK;

// int normalEncoderCounter = 0;
// int normalEncoderCLK;
// int prevNormalEncoderCLK;

// The sublists are the encoders in this order: verticalLeft, verticalRight, normal
// The content of these sunlists are: [inputClock, inputDirection, counter, currentClockState, prevClockState]
int encoderData[][5] = {
  {inputClkVerticalLeftEncoder, inputDirVerticalLeftEncoder, 0, 0, 0}, 
  {inputClkVerticalRightEncoder, inputDirVerticalRightEncoder, 0, 0, 0}, 
  {inputClkNormalEncoder, inputDirNormalEncoder, 0, 0, 0}
}


void encoderSetup(){
  Serial.println("Encoder initializing");
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
    encoderData[i][3] = digitalRead(encoderData[i][0])
    
    // If the previous and the current state of the inputCLK are different then a pulse has occured
    if (encoderData[i][3] != encoderData[i][4]){
      
      // If the inputDT state is different than the inputCLK state then 
      // the encoder is rotating counterclockwise
      if (digitalRead(encoderData[i][1]) != encoderData[i][4]){
        
        encoderData[i][2] --;
      
      } else {
      
        encoderData[i][2] ++;
      
      }
    }
    encoderData[i][4] = encoderData[i][3];
  }

}

void processEncoderData(){
  int encoderDist = 10;

  int verticalLeftEncoder; // Should be the number of ticks multi. by the wheel circumference
  int prevVerticalLeftEncoder;
  int verticalRightEncoder;
  int prevVerticalRightEncoder;
  int normalEncoder;
  int prevNormalEncoder;

  int leftChange;
  int rightChange;
  int changeOrientation;
  int orientationRads;

  int rawHorizontalChange;
  int horizontalChange;

  int centerChange;
  int globalXPos;
  int globalYPos;

  // Get current position
  leftChange = verticalLeftEncoder-prevVerticalLeftEncoder;
  rightChange = verticalRightEncoder-prevVerticalRightEncoder;

  // Calculate angle
  changeOrientation = (leftChange-rightChange)/encoderDist; //
  orientationRads += changeOrientation; //

  // Get x and y component of the motion
  rawHorizontalChange = normalEncoder - prevNormalEncoder;
  horizontalChange = rawHorizontalChange - (changeOrientation*horizontalEncoderTickPerDegreeOffset)
  centerChange =  = (rightChange + leftChange)/2; //
  globalXPos += centerChange*sin(orientationRads) + horizontalChange*cos(orientationRads);
  globalYPos += centerChange*cos(orientationRads) - horizontalChange*sin(orientationRads);

}


/* 
#define inputCLK 48
#define inputDT 49
 
// 40 ticks per revolution
int counter = 0;
int currentStateCLK;
int previousStateCLK;
String encdir ="";

void encoderSetup(){
  Serial.println("Encoder initializing");
  // set the encoder pins as inputs
  pinMode (inputCLK,INPUT);
  pinMode (inputDT,INPUT);

  // Read the initial state of inputCLK
  // Assign to previousStateCLK variable
  previousStateCLK = digitalRead(inputCLK);
  Serial.println("Encoder setup finished");
}


void readEncoders(){
  // Read the current state of inputCLK
  currentStateCLK = digitalRead(inputCLK);
  
  // If the previous and the current state of the inputCLK are different then a pulse has occured
  if (currentStateCLK != previousStateCLK){ 
     
   // If the inputDT state is different than the inputCLK state then 
   // the encoder is rotating counterclockwise
   if (digitalRead(inputDT) != currentStateCLK) { 
     counter --;
     encdir ="CCW";
     
   } else {
     // Encoder is rotating clockwise
     counter ++;
     encdir ="CW";
     
   }
   Serial.print("Direction: ");
   Serial.print(encdir);
   Serial.print(" -- Value: ");
   Serial.println(counter);
  } 
  // Update previousStateCLK with the current state
  previousStateCLK = currentStateCLK;  
}
*/

// All angles are in radians
// int distLeftX = 0;
// int distRightX = 0;
// int distBottomY = 0;
// int xToCenterDist = 10;
// int yToCenterDist = 10;
// int angle;

// int distMiddle;
// int pastOrientation = 0;
// int newOrientation = 0;
// int pastX = 0;
// int pastY = 0;
// int currentX = 0;
// int currentY = 0;

// angle = (distRightX-distLeftX)/xToCenterDist;
// distMiddle = (distRightX-distLeftX)/2 ;
// currentX = pastX + ((distMiddle/angle)*sin(angle)/cos(angle))*cos(pastOrientation + angle/2);
// currentY = pastX + ((distMiddle/angle)*sin(angle)/cos(angle))*cos(pastOrientation + angle/2);
// newOrientation = pastOrientation + angle;
// pastX = currentX;
// pastY = currentY; 
