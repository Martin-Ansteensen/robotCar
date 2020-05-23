/*  
  Made for arduino mega
*/




void setup(){

  Serial.begin(9600);
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
