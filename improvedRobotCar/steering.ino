//  Motor variables

// Front left
#define enBLeft 7 
#define in3Left 24
#define in4Left 22 
 
//  Back left
#define enALeft 6
#define in1Left 28
#define in2Left 26


//  Front right
#define enARight 5
#define in1Right 25
#define in2Right 23

//  Back right
#define enBRight 4
#define in3Right 29
#define in4Right 27


// Integral has to be very low until the gyro starts at 0.00
int calculatedSpeed[4]; // Front left, front right, back left, back right
float calculatedPID = 0;
//float Kp = 120 , Ki = 0.05, Kd = 60;
float Kp = 2 , Ki = 0, Kd = 0.5;
float total = 0;
float derivative, previousError, integral, error;

// Motor chart
//Back tracking enocoder wheel
//3       4
// Middel encoder
// 1      2


void motorSetup(){
  Serial.println("Initializing motors");
  //  Set all the motor control pins as outputs
  
  //  Left motors
  pinMode(enALeft, OUTPUT);
  pinMode(enBLeft, OUTPUT);
  pinMode(in1Left, OUTPUT);
  pinMode(in2Left, OUTPUT);
  pinMode(in3Left, OUTPUT);
  pinMode(in4Left, OUTPUT);

  //  Right motors
  pinMode(enARight, OUTPUT);
  pinMode(enARight, OUTPUT);
  pinMode(in1Right, OUTPUT);
  pinMode(in2Right, OUTPUT);
  pinMode(in3Right, OUTPUT);
  pinMode(in4Right, OUTPUT);

}

float controllerPID (float previousError){
  getRotation();
  error = (desiredAngelScale180-robotAngelScale180);  
  integral += error; // (error*runtime); 
  derivative = (error-previousError); // (error-previousError)/runtime;
  total = error*Kp+ integral*Ki + derivative*Kd;
  return total;
}

void runPID(){
  // int InitTime = micros();
  calculatedPID = controllerPID(previousError);
  previousError = error;
  //runtime = float ( micros() - InitTime ) / 1000000.0;
}

void calculateSpeed(int mSpeed, float angle, double error){
  calculatedSpeed[0] = (mSpeed*sin(angle+M_PI/4)*sqrt(2)) - error; // Front left
  calculatedSpeed[1] = (mSpeed*cos(angle+M_PI/4)*sqrt(2)) + error; // Front right
  calculatedSpeed[2] = (mSpeed*cos(angle+M_PI/4)*sqrt(2)) - error; // Back left
  calculatedSpeed[3] = (mSpeed*sin(angle+M_PI/4)*sqrt(2)) + error; // Back right
}

// Angle in rad and speed from 0-255
void driveMeccanumGyro(float angle, float mSpeed){ 
    runPID();
    calculateSpeed(mSpeed, angle, calculatedPID);

    //  Scale the speeds in case one of them exceeds the motors limits
    double largest = abs(calculatedSpeed[0]);
    for (int i = 1; i < 4; i++){
      if (abs(calculatedSpeed[i]) > largest){
        largest = abs(calculatedSpeed[i]);
      }
    }
    //  Only scale speeds if the largest "speed" exceeds limits
    if (largest > 255.0){
      // Serial.println("Scaling down motor speeds");
      for (int i = 0; i < 4; i++){
        calculatedSpeed[i] = calculatedSpeed[i] * (255/largest); 
      }
    }
    
    motorDirection();
    analogWrite(enBLeft, abs(calculatedSpeed[0])); // Front left
    analogWrite(enARight, abs(calculatedSpeed[1])); // Front right
    analogWrite(enALeft, abs(calculatedSpeed[2])); // Back left
    analogWrite(enBRight, abs(calculatedSpeed[3])); // Back right
/*
   Serial.print(" | motor1  = "); Serial.print(calculatedSpeed[0]);
   Serial.print(" | motor2  = "); Serial.print(calculatedSpeed[1]);
   Serial.print(" | motor3  = "); Serial.print(calculatedSpeed[2]);
   Serial.print(" | motor4  = "); Serial.print(calculatedSpeed[3]);
   Serial.print(" | desired angel  = "); Serial.print(desiredAngelScale180*57.2957);
   Serial.print(" | robot angel  = "); Serial.print(robotAngelScale180);
   Serial.print(" | error  = "); Serial.print(calculatedPID);
   Serial.println("");  
*/
 }


void driveMeccanum(float angle, float mSpeed, float duration){

    calculateSpeed(mSpeed, radians(angle), 0.0);
    motorDirection();
    analogWrite(enBLeft, abs(calculatedSpeed[0])); // Front left
    analogWrite(enARight, abs(calculatedSpeed[1])); // Front right
    analogWrite(enALeft, abs(calculatedSpeed[2])); // Back left
    analogWrite(enBRight, abs(calculatedSpeed[3])); // Back right

    Serial.print(" | motor1  = "); Serial.print(calculatedSpeed[0]);
    Serial.print(" | motor2  = "); Serial.print(calculatedSpeed[1]);
    Serial.print(" | motor3  = "); Serial.print(calculatedSpeed[2]);
    Serial.print(" | motor4  = "); Serial.print(calculatedSpeed[3]);
    Serial.println("");
      
    delay(duration);
    DisableAll();

 }

void motorDirection(){
  if (calculatedSpeed[0] > 0){
    FrontLeftForward();
  } else {
    FrontLeftBackward();
  }
  
  if (calculatedSpeed[1] > 0){
    FrontRightForward();
  } else {
    FrontRightBackward();
  }
  
  if (calculatedSpeed[2] > 0){
    BackLeftForward();
  } else {
    BackLeftBackward();
  }
  
  if (calculatedSpeed[3] > 0){
    BackRightForward();
  } else {
    BackRightBackward();
  }
}

// Front left
void FrontLeftForward(){
  digitalWrite(in3Left, HIGH);
  digitalWrite(in4Left, LOW);
}
void FrontLeftBackward(){
  digitalWrite(in3Left, LOW);
  digitalWrite(in4Left, HIGH);
}

// Front right
void FrontRightForward(){
  digitalWrite(in1Right, LOW);
  digitalWrite(in2Right, HIGH);
}
void FrontRightBackward(){
  digitalWrite(in1Right, HIGH);
  digitalWrite(in2Right, LOW);
}

// Back left
void BackLeftForward(){
  digitalWrite(in1Left, LOW);
  digitalWrite(in2Left, HIGH);
}
void BackLeftBackward(){
  digitalWrite(in1Left, HIGH);
  digitalWrite(in2Left, LOW);
}

// Back right
void BackRightForward(){
  digitalWrite(in3Right, HIGH);
  digitalWrite(in4Right, LOW);
}
void BackRightBackward(){
  digitalWrite(in3Right, LOW);
  digitalWrite(in4Right, HIGH);
}

void DisableAll(){
  digitalWrite(in1Left, LOW);
  digitalWrite(in2Left, LOW);
  
  digitalWrite(in1Right, LOW);
  digitalWrite(in2Right, LOW);

  digitalWrite(in3Right, LOW);
  digitalWrite(in4Right, LOW);
   
  digitalWrite(in3Left, LOW);
  digitalWrite(in4Left, LOW);
}
