#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(49, 48);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
double  driveJoystickX,  driveJoystickY, rotationJoystickX, rotationJoystickY;
double driveAngel, magnitude;
float pastDesiredAngel = 0;

float desiredAngelScale360;
float desiredAngelScale180 = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  //  byte j2PotX;
  //  byte j2PotY;
  //  byte j1Button;
  //  byte j2Button;
  //  byte up;
  //  byte down;
  //  byte left;
  //  byte right;
  //  byte pot1;
  //  byte pot2;
  //  byte switch1;
  //  byte switch2;
};
Data_Package data; //Create a variable with the above structure

void recieverSetup() {
  Serial.println("Reciever running");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  //radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); //  Set the module as receiver
  resetData();
}

bool recieveData() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
    return true;
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }
  // Print the data in the Serial Monitor
//    Serial.print("j1PotX: ");
//    Serial.print(data.j1PotX);
//    Serial.print("; j1PotY: ");
//    Serial.println(data.j1PotY);

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

void calculateFromRadio() {
  driveJoystickX = map(data.j1PotX, 0, 255, -123, 122);
  driveJoystickY = map(data.j1PotY, 0, 255, -123, 122);
  driveAngel = -1*atan2(driveJoystickX, driveJoystickY) - (M_PI / 2);


  //  rotationJoystickX = map(data.j2PotX, 0, 255, -100, 100);
  //  rotationJoystickY = map(data.j2PotY, 0, 255, -100, 100);
  //  desiredAngelScale180 = atan2(rotationJoystickX, rotationJoystickY)-(M_PI/2);
  //  if (desiredAngelScale180 < 0){ // Map the angel from -PI, 0, PI to 0, 2PI
  //    desiredAngelScale180 = (desiredAngelScale180 - 0) * (-1*M_PI) / (-1*M_PI) + 2*M_PI;
  //  }
  //  desiredAngelScale180 = desiredAngelScale180-pastDesiredAngel;

  desiredAngelScale180 = 0;
  
  if (driveAngel < 0) { // Map the angel from -PI, 0, PI to 0, 2PI
    driveAngel = (driveAngel - 0) * (-1 * M_PI) / (-1 * M_PI) + 2 * M_PI;
  }
  magnitude = abs(max(abs(driveJoystickX), abs(driveJoystickY)))*2;

/*
  Serial.print(" | org x  = "); Serial.print(data.j1PotX);
  Serial.print(" | org y  = "); Serial.print(data.j1PotY);
  Serial.print(" | x  = "); Serial.print(driveJoystickX);
  Serial.print(" | y  = "); Serial.print(driveJoystickY);

  Serial.print(" | angel  = "); Serial.print(driveAngel * 57.2957);
  Serial.print(" | magnitude  = "); Serial.print(magnitude);
  Serial.println("");*/

}

void radioControl() {
  driveMeccanumGyro(driveAngel, magnitude);
}



void resetData() {
  data.j1PotX = 127;
  data.j1PotY = 127;
  //  data.j2PotX = 127;
  //  data.j2PotY = 127;
  //  data.j1Button = 1;
  //  data.j2Button = 1;
  //  data.up = 1;
  //  data.down = 1;
  //  data.left = 1;
  //  data.right = 1;
  //  data.pot1 = 1;
  //  data.pot2 = 1;
  //  data.switch1 = 1;
  //  data.switch2 = 1;
}
