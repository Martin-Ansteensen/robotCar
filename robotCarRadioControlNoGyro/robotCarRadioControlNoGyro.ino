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
  radioControl();
  
}
