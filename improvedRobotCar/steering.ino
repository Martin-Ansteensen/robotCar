//  Motor variables
//  Driver Left

// Front left
const byte enBL = 3; //3
const byte in3L = 23; 
const byte in4L = 22; 
 
//  Back left
const byte enAL = 5; //7
const byte in1L = 25; // Any digital
const byte in2L = 24; // Any digital


//  Driver Right

//  Front right
const byte enAR = 7; //5
const byte in1R = 29;
const byte in2R = 28;

//  Back right
const byte enBR = 6; // 6
const byte in3R = 26; 
const byte in4R = 27;

//20, 0, 10
//60, 0, 40
// Integral has to be very low until the gyro starts at 0.00
int calculatedSpeed[4];
float cPID = 0;
float Kp = 120 , Ki = 0.05, Kd = 60;
float total = 0;
float derivative, previousError, integral, runtime, error;

// Motor chart
//Back tracking enocoder wheel
//3       4
// Middel encoder
// 1      2



void motorSetup(){
  Serial.println("Initializing motors");
  //  Set all the motor control pins to outputs
  
  //  Left motors
  pinMode(enAL, OUTPUT);
  pinMode(enBL, OUTPUT);
  pinMode(in1L, OUTPUT);
  pinMode(in2L, OUTPUT);
  pinMode(in3L, OUTPUT);
  pinMode(in4L, OUTPUT);

  //  Right motors
  pinMode(enAR, OUTPUT);
  pinMode(enBR, OUTPUT);
  pinMode(in1R, OUTPUT);
  pinMode(in2R, OUTPUT);
  pinMode(in3R, OUTPUT);
  pinMode(in4R, OUTPUT);

}

float controllerPID (float previousError, int runtime){
  getRotation();
  error = desiredAngelScale180-robotAngelScale180;  
  integral += error; //(error*runtime); 
  derivative = (error-previousError); // / runtime;
  total = error*Kp+ integral*Ki + derivative*Kd;
  return total;
}

void runPID(){
  int InitTime = micros();
  cPID = controllerPID(previousError, runtime);
  previousError = error;
  runtime = float ( micros() - InitTime ) / 1000000.0;
}

void calculateSpeed(int mSpeed, float angle, double error){
  calculatedSpeed[0] = (mSpeed*sin(angle+M_PI/4)*sqrt(2)) - error; // Front left
  calculatedSpeed[1] = (mSpeed*cos(angle+M_PI/4)*sqrt(2))+error; // Front right
  calculatedSpeed[2] = (mSpeed*cos(angle+M_PI/4)*sqrt(2))- error; // Back left
  calculatedSpeed[3] = (mSpeed*sin(angle+M_PI/4)*sqrt(2))+error; // Back right
}



void driveMeccanumGyro(float angle, float mSpeed){
    //desiredAngelScale180 = 0;
    runPID();
    calculateSpeed(mSpeed, angle, cPID);

    // Scale the speeds in case one of them exceeds the motors limits
    double largest = abs(calculatedSpeed[0]);
    for (int i = 1; i < 4; i++){
      if (abs(calculatedSpeed[i]) > largest){
        largest = abs(calculatedSpeed[i]);
      }
    }
//     Only scale speeds if the largest "speed" exceeds limits
    if (largest > 255.0){
      // Serial.println("Scaling down motor speeds");
      for (int i = 0; i < 4; i++){
        calculatedSpeed[i] = calculatedSpeed[i] * (255/largest); 
      }
    }
    
    motorDirection();
    analogWrite(enBL, abs(calculatedSpeed[0])); // Front left
    analogWrite(enAR, abs(calculatedSpeed[1])); // Front right
    analogWrite(enAL, abs(calculatedSpeed[2])); // Back left
    analogWrite(enBR, abs(calculatedSpeed[3])); // Back right
//
//    Serial.print(" | motor1  = "); Serial.print(calculatedSpeed[0]);
//    Serial.print(" | motor2  = "); Serial.print(calculatedSpeed[1]);
//    Serial.print(" | motor3  = "); Serial.print(calculatedSpeed[2]);
//    Serial.print(" | motor4  = "); Serial.print(calculatedSpeed[3]);
//    //Serial.print(" | desired angel  = "); Serial.print(desiredAngelScale180*57.2957);
//    //Serial.print(" | robot angel  = "); Serial.print(robotAngelScale180*57.2957);
//    Serial.print(" | error  = "); Serial.print(cPID);
//    Serial.println("");  

 }


void driveMeccanum(float angle, float mSpeed, float duration){

    calculateSpeed(mSpeed, radians(angle), 0.0);
    motorDirection();
    analogWrite(enBL, abs(calculatedSpeed[0])); // Front left
    analogWrite(enAR, abs(calculatedSpeed[1])); // Front right
    analogWrite(enAL, abs(calculatedSpeed[2])); // Back left
    analogWrite(enBR, abs(calculatedSpeed[3])); // Back right

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
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
}
void FrontLeftBackward(){
  digitalWrite(in3L, HIGH);
  digitalWrite(in4L, LOW);
}

// Front right
void FrontRightForward(){
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
}
void FrontRightBackward(){
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
}

// Back left
void BackLeftForward(){
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
}
void BackLeftBackward(){
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
}

// Back right
void BackRightForward(){
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, HIGH);
}
void BackRightBackward(){
  digitalWrite(in3R, HIGH);
  digitalWrite(in4R, LOW);
}

void DisableAll(){
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, LOW);
  
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, LOW);

  digitalWrite(in3R, LOW);
  digitalWrite(in4R, LOW);
   
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, LOW);
}
