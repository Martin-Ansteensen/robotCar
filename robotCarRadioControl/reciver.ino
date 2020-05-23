#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte up;
  byte down;
  byte left;
  byte right;
  byte pot1;
  byte pot2;
  byte switch1;
  byte switch2;
};
Data_Package data; //Create a variable with the above structure

void recieverSetup(){
  Serial.println("Reciever running");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
}

void recieveData() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }
  // Print the data in the Serial Monitor
//  Serial.print("j1PotX: ");
//  Serial.print(data.j1PotX);
//  Serial.print("; j1PotY: ");
//  Serial.print(data.j1PotY);
//  Serial.print("; j1Button: ");
//  Serial.println(data.j1Button);
//  
//  Serial.print("; j2PotX: ");
//  Serial.print(data.j2PotX); 
//  Serial.print("; j2PotY: ");
//  Serial.print(data.j2PotY);
//  Serial.print("; j2Button: ");
//  Serial.println(data.j2Button);
}

void radioControl(){
  int j1PotX = map(data.j2PotX, 0, 255, -1, 1);
  int j1PotY = map(data.j2PotY, 0, 255, -1, 1);
  int driveAngel = round(atan2(j1PotX, j1PotY)*57.295779);
  int magnitude = j1PotX/sin(atan2(j1PotX, j1PotY));
  magnitude = map(magnitude, 0, 1.42, 0, 80);
  driveMeccanumGyroWithRadio(driveAngel, magnitude);  
}



void resetData() {
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.up = 1;
  data.down = 1;
  data.left = 1;
  data.right = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.switch1 = 1;
  data.switch2 = 1;
}
