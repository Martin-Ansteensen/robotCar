
/*  
  Made for arduino mega
*/

void setup(){

  Serial.begin(115200);
  Serial.println("Robotcar initializing");
  gyroSetup();
  motorSetup();
  recieverSetup();
  Serial.println("Robotcar finished initializing");

}
 
void loop(){
  recieveData();
  calculateFromRadio();
  radioControl();
  //driveMeccanumGyro(0, 75);
  //driveMeccanum(0, 200, 10000);
}
