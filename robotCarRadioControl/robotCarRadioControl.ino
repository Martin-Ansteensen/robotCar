/*  
  Made for arduino mega
*/




void setup(){

  Serial.begin(115200);
  Serial.println("Robotcar initializing");
  motorSetup();
  gyroSetup();
  recieverSetup();
  Serial.println("Robotcar finished initializing");

}
 
void loop(){
  recieveData(); 
  radioControl();
  
}
