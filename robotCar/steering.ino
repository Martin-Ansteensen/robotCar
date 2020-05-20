
double calculatedSpeed[4];
float cPID = 0;
float Kp = 1 , Ki = 0.3, Kd = 0.1;
float error, followAngel180, followAngel360, robotAngel180, robotAngel360, derivative, previousError, integral, runtime;

// Encoder wheels
// 3          4
// 1          2
// Arduino Ports

void calculateSpeed(int mSpeed, int angle, float error){
  //if (angle < 0){ // TRNEGS IKKE
    //Serial.println(angle);
    //angle = map(angle, -360, 0, 0, 360);
    //Serial.println(angle);
    //Serial.println("");
  //}
  calculatedSpeed[0] = (mSpeed*sin(radians(angle)+PI/4)*sqrt(2)) - error; // enAL
  calculatedSpeed[1] = (mSpeed*cos(radians(angle)+PI/4)*sqrt(2))+error; // enAR
  calculatedSpeed[2] = (mSpeed*cos(radians(angle)+PI/4)*sqrt(2))- error; // enBL
  calculatedSpeed[3] = (mSpeed*sin(radians(angle)+PI/4)*sqrt(2))+error; // enBR
}

float controllerPID (float previousError, int runtime){
  getRotation();
  robotAngel180 = rotation180;
  robotAngel360 = rotation360;
  if (followAngel180 > 0){
    error = followAngel180-robotAngel180;
  } else {
    error = followAngel180-robotAngel180;  
  }
  //integral += (error*runtime); 
  //derivative = (error-previousError) / runtime;
  return error*Kp;// + integral*Ki + derivative*Kd;
}

void runPID(){
  //int InitTime = micros();
  cPID = controllerPID(0, 0);
  //previousError = error;
  //runtime = float ( micros() - InitTime ) / 1000000.0;
}

// Motorspeed is 60 and something, this is not mapped yet. Should map input from 0-100 to motor values
void driveMeccanumGyro(int angle, float mSpeed, int distance){
  getRotation();
  delay(1000);
  followAngel180 = rotation180;
  //followAngel360 = rotation360; 
  
  //mSpeed = map(mSpeed, 0, 100, 60, 100);
  
  int counter = 0;
  while(counter < distance){
    runPID();
    getRotation();
    robotAngel180 = rotation180;
    //robotAngel360 = rotation360; 
     
    //error = abs(followAngel180)+(robotAngel180); ikke bruk
    calculateSpeed(mSpeed, angle, cPID);

    // Scale the speeds in case one of them exceeds the motors limits
    double largest = abs(calculatedSpeed[0]);
    for (int i = 1; i < 4; i++){
      if (abs(calculatedSpeed[i]) > largest){
        largest = abs(calculatedSpeed[i]);
      }
    }
    // Only scale speeds if the largest "speed" exceeds limits
    if (largest > 100.0){
      Serial.println("Scaling down motor speeds");
      for (int i = 0; i < 4; i++){
        calculatedSpeed[i] = calculatedSpeed[i] / largest; 
      }
    }
    
    motorDirection();
    analogWrite(enAL, abs(calculatedSpeed[0]));
    analogWrite(enAR, abs(calculatedSpeed[1]));
    analogWrite(enBL, abs(calculatedSpeed[2]));
    analogWrite(enBR, abs(calculatedSpeed[3]));

    Serial.print(" | motor1  = "); Serial.print(calculatedSpeed[0]);
    Serial.print(" | motor2  = "); Serial.print(calculatedSpeed[1]);
    Serial.print(" | motor3  = "); Serial.print(calculatedSpeed[2]);
    Serial.print(" | motor4  = "); Serial.print(calculatedSpeed[3]);
    Serial.print(" | error  = "); Serial.print(error);
    Serial.print(" | robot angel180  = "); Serial.print(robotAngel180);
    //Serial.print(" | robot angel360  = "); Serial.print(robotAngel360);
    //Serial.print(" | follow angel 360 = "); Serial.print(followAngel360);
    Serial.print(" | follow angel 180 = "); Serial.print(followAngel180);
    Serial.print(" | cPID = "); Serial.print(cPID);
    Serial.println("");  
    counter++;
  }
  DisableAll();
}


void motorDirection(){
  if (calculatedSpeed[0] > 0){
    MotorOneForward();
  } else {
    MotorOneBackward();
  }
  
  if (calculatedSpeed[1] > 0){
    MotorTwoForward();
  } else {
    MotorTwoBackward();
  }
  
  if (calculatedSpeed[2] > 0){
    MotorThreeForward();
  } else {
    MotorThreeBackward();
  }
  
  if (calculatedSpeed[3] > 0){
    MotorFourForward();
  } else {
    MotorFourBackward();
  }
}

// Front left
void MotorOneForward(){
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
}
void MotorOneBackward(){
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
}

// Front right
void MotorTwoForward(){
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
}
void MotorTwoBackward(){
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
}

// Back left
void MotorThreeForward(){
  digitalWrite(in3L, HIGH);
  digitalWrite(in4L, LOW);
}
void MotorThreeBackward(){
  digitalWrite(in3L, LOW);
  digitalWrite(in4L, HIGH);
}

// Back right
void MotorFourForward(){
  digitalWrite(in3R, LOW);
  digitalWrite(in4R, HIGH);
}
void MotorFourBackward(){
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

// Motorspeed is 60 and something, this is not mapped yet. Should map input from 0-100 to motor values
void driveMeccanum(int angle, float mSpeed, int distance){
  
  int counter = 0;
  while(counter < distance){
    calculateSpeed(mSpeed, angle, 0);
    
    // Scale the speeds in case one of them exceeds the motors limits
    double largest = abs(calculatedSpeed[0]);
    for (int i = 1; i < 4; i++){
      if (abs(calculatedSpeed[i]) > largest){
        largest = abs(calculatedSpeed[i]);
      }
    }
    // Only scale speeds if the largest "speed" exceeds limits
    if (largest > 100.0){
      Serial.println("Scaling down motor speeds");
      for (int i = 0; i < 4; i++){
        calculatedSpeed[i] = calculatedSpeed[i] / largest; 
      }
    }
    
    motorDirection();
    analogWrite(enAL, abs(calculatedSpeed[0]));
    analogWrite(enAR, abs(calculatedSpeed[1]));
    analogWrite(enBL, abs(calculatedSpeed[2]));
    analogWrite(enBR, abs(calculatedSpeed[3]));

    Serial.print(" | motor1  = "); Serial.print(calculatedSpeed[0]);
    Serial.print(" | motor2  = "); Serial.print(calculatedSpeed[1]);
    Serial.print(" | motor3  = "); Serial.print(calculatedSpeed[2]);
    Serial.print(" | motor4  = "); Serial.print(calculatedSpeed[3]);  
    Serial.println("");  
    counter++;
  }
  DisableAll();
}
