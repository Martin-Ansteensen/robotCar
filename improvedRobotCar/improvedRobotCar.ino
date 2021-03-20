
/*  
  Made for Arduino Mega, but can also be used 
  with Arduino Due for better performance
*/

void setup(){

  //Serial.begin(9600);
  Serial.begin(115200);
  Serial.println("Robotcar initializing");
  recieverSetup();
  gyroSetup();
  motorSetup();
  //encoderSetup();
  Serial.println("Robotcar finished initializing");

}
 
void loop(){
  //driveMeccanumGyro(0, 120);
  
  //readEncoders();
  //processEncoderData();
  recieveData();
  calculateFromRadio();
  radioControl();
  
  //getRotation();

   
}
