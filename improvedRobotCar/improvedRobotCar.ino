
/*  
  Made for Arduino Mega, but can also be used 
  with Arduino Due for better performance
*/
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds


void setup(){

  Serial.begin(9600);
  Serial.println("Robotcar initializing");
  recieverSetup();
  gyroSetup();
  motorSetup();
  encoderSetup();
  Serial.println("Robotcar finished initializing");
  startMillis = millis();  //initial start time
}
 
void loop(){
  //driveMeccanumGyro(0, 120);
  currentMillis = millis();
  if (currentMillis - startMillis >= period){  //test whether the period has elapsed
    readEncoders();
    processEncoderData(); 
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
 
  recieveData();
  calculateFromRadio();
  radioControl(); 
}
