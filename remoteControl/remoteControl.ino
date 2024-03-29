#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// Define the digital inputs
#define j1B_PIN 6  // Left joystick button
#define j1X_PIN A1 // Left joystick x axis
#define j1Y_PIN A0 // Left joystick y axis

#define j2B_PIN 9  // Right joystick button
#define j2X_PIN A2 // Rigth joystick x axis
#define j2Y_PIN A3 // Right joystick y axis

#define UP_PIN 2  // Up button
#define DOWN_PIN 5 // Down button
#define LEFT_PIN 3 // Left button
#define RIGHT_PIN 4  // Right button


#define POT_LEFT_PIN A4  // Left pot
#define POT_RIGHT_PIN A5 // Right pot

#define SWITCH_UP_PIN A7  // Switch up pin
#define SWITCH_DOWN_PIN A6 // Switch down pin


RF24 radio(7, 8);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Radio-address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j2PotX;
  byte j2PotY;
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
Data_Package data;

void setup() {
  Serial.begin(9600);
  Serial.println("Transmitter running");  
  // Start the radio broadcasting
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  //radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  
  // Activate the Arduino internal pull-up resistors
  pinMode(j1B_PIN, INPUT_PULLUP);
  pinMode(j2B_PIN, INPUT_PULLUP);
  
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);
}
void loop() {
  // Read all analog inputs and map them to one Byte value
  data.j1PotX = map(analogRead(j1X_PIN), 0, 1023, 0, 255);
  data.j1PotY = map(analogRead(j1Y_PIN), 0, 1023, 0, 255);
  
  data.j2PotX = map(analogRead(j2X_PIN), 0, 1023, 0, 255);
  data.j2PotY = map(analogRead(j2Y_PIN), 0, 1023, 0, 255);

//   data.pot1 = map(analogRead(POT_LEFT_PIN), 0, 1023, 0, 255);
//   data.pot2 = map(analogRead(POT_RIGHT_PIN), 0, 1023, 0, 255);
//
//   if (analogRead(SWITCH_UP_PIN) < 5){
//     data.switch1 = 0;
//     data.switch2 = 1;
//   } else if (analogRead(SWITCH_DOWN_PIN) < 5){
//     data.switch2 = 0;
//     data.switch1 = 1;
//   } else {
//     data.switch1 = 1;
//     data.switch2 = 1;
//   }


  // // Read all digital inputs
  // data.j1Button = digitalRead(j1B_PIN);
  // data.j2Button = digitalRead(j2B_PIN);

  // data.up = digitalRead(UP_PIN);
  // data.down = digitalRead(DOWN_PIN);
  // data.left = digitalRead(LEFT_PIN);
  // data.right = digitalRead(RIGHT_PIN);

  
  // Print data
   Serial.print(F("L_JOY XY: ")); Serial.print(data.j1PotX); Serial.print(F(" , ")); Serial.print(data.j1PotY); Serial.print(F(" , ")); 
  // Serial.print("L_B: "); Serial.print(data.j1Button); Serial.print(" , ");
   Serial.print(F("R_JOY XY:   ")); Serial.print(data.j2PotX); Serial.print(F(" , ")); Serial.print(data.j2PotY); Serial.print(F(" , "));
  // Serial.print("R_B:   "); Serial.print(data.j2Button); Serial.print(" , ");
  // Serial.print("UP: "); Serial.print(data.up); Serial.print(" , ");
  // Serial.print("DOWN: "); Serial.print(data.down); Serial.print(" , ");
  // Serial.print("LEFT: "); Serial.print(data.left); Serial.print(" , ");
  // Serial.print("RIGHT: "); Serial.print(data.right); Serial.print(" , ");
  // Serial.print("L_POT: "); Serial.print(data.pot1); Serial.print(" , "); 
  // Serial.print("R_POT: "); Serial.print(data.pot2); Serial.print(" , "); 
  // Serial.print("SW_FOR: "); Serial.print(data.switch1); Serial.print(" , "); 
  // Serial.print("SW_BAC: "); Serial.print(data.switch2); 
   Serial.println(); 

  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));
}
