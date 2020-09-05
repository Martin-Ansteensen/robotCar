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

// All angles are in radians
int distLeftX = 0;
int distRightX = 0;
int distBottomY = 0;
int xToCenterDist = 10;
int yToCenterDist = 10;
int angle;

int distMiddle;
int pastOrientation = 0;
int newOrientation = 0;
int pastX = 0;
int pastY = 0;
int currentX = 0;
int currentY = 0;


currentX = pastX + ((distMiddle/angle)*sin(angle)/cos(angle))*cos(pastOrientation + angle/2);
currentY = pastX + ((distMiddle/angle)*sin(angle)/cos(angle))*cos(pastOrientation + angle/2);
newOrientation = pastOrientation + angle;
pastX = currentX;
pastY = currentY; 

