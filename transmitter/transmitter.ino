#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// Define the digital inputs
#define j1B_PIN 6  // Left joystick button lef
#define j1X_PIN A1 // Left joystick x axis
#define j1Y_PIN A0 // Left joystick y axis

#define j2B_PIN 9  // Right joystick button
#define j2X_PIN A2 // Left joystick x axis
#define j2Y_PIN A3 // Left joystick y axis


float elapsedTime, currentTime, previousTime;
int c = 0;

RF24 radio(7, 8);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
};
Data_Package data; //Create a variable with the above structure
void setup() {
  Serial.begin(9600);
  Serial.println("Transmitter running");
   
  // Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  
  // Activate the Arduino internal pull-up resistors
  pinMode(j1B_PIN, INPUT_PULLUP);
  pinMode(j2B_PIN, INPUT_PULLUP);

  // Set initial default values
  data.j1PotX = 127; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;

}
void loop() {
  // Read all analog inputs and map them to one Byte value
  data.j1PotX = map(analogRead(j1X_PIN), 0, 1023, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.j1PotY = map(analogRead(j1Y_PIN), 0, 1023, 0, 255);
  data.j2PotX = map(analogRead(j2X_PIN), 0, 1023, 0, 255);
  data.j2PotY = map(analogRead(j2Y_PIN), 0, 1023, 0, 255);

  // Read all digital inputs
  data.j1Button = digitalRead(j1B_PIN);
  data.j2Button = digitalRead(j2B_PIN);
  
  // Print data
  Serial.print("Left joystick XY: "); Serial.print(data.j1PotX); Serial.print(" , "); Serial.print(data.j1PotY); 
  Serial.print(" , ");Serial.print("Left joystick button:   "); Serial.print(data.j1Button); Serial.print(" , ");
  Serial.print("Right joystick XY:   "); Serial.print(data.j2PotX); Serial.print(" ,   "); Serial.print(data.j2PotY); Serial.print(" , ");
  Serial.print("Right joystick button:   "); Serial.println(data.j2Button); 
  

  
  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));
}
