
/*  
  Made for Arduino Mega, but can also be used 
  with Arduino Due for better performance
*/

void setup(){

  Serial.begin(9600);
  Serial.println("Robotcar initializing");
  //gyroSetup();
  //motorSetup();
  //recieverSetup();
  encoderSetup();
  Serial.println("Robotcar finished initializing");

}
 
void loop(){
  processEncoderData();
  //recieveData();
  //calculateFromRadio();
  //radioControl();
 
}
