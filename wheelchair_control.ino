// Library to make a Software UART
#include <SoftwareSerial.h>

// Pin locations
#define RX A4
#define TX A5

#define BAUDRATE 9600

// Length of serial message
#define SERIAL_LENGTH 6

// Data needed to control the motors
struct moveVals {
  int velMagnitude; // 2 bytes
  int velDirection; // 2
  int stopTrue;     // 2
};

// Struct is overlaid onto an array for conversion from 
// the binary serial message
union serialInput {
  moveVals moveData;
  byte serialMessage[SERIAL_LENGTH];
};

// Working instance of union
serialInput inputData;

byte serialData[SERIAL_LENGTH];
boolean newData = false;
boolean askForData = true;

// Instantiation of a Software UART
SoftwareSerial BTSerial(RX, TX); // (RX, TX)

void setup() {  

  // Start Serial Monitor for feedback
  Serial.begin(BAUDRATE);
  while(!Serial);
  
  // Start HM-10
  BTSerial.begin(BAUDRATE);
  while(!BTSerial);
  
  Serial.println("Receiving data:");
}

void loop() {
  requestData();
  receiveData();
  displayData();

  delay(1000);
}

void requestData()
{
  if (askForData) 
  {
    Serial.print("<M>");
    askForData = false;
  }
}

void receiveData() 
{
  if (BTSerial.available() < SERIAL_LENGTH) 
  {
    Serial.println("Error");
  }
  
  for (byte n = 0; n < SERIAL_LENGTH; n++) 
  {
    serialData[n] = BTSerial.read();
  }
  
  for (byte n = 0; n < SERIAL_LENGTH; n++)
  {
    inputData.serialMessage[n] = serialData[n];
  }
  
  newData = true;
}

void displayData()
{
  if (newData == false)
  {
    return;
  }
  Serial.print("<Data: ");
  Serial.print(inputData.moveData.velMagnitude);
  Serial.print(" ");
  Serial.print(inputData.moveData.velDirection);
  Serial.print(" ");
  Serial.print(inputData.moveData.stopTrue);
  Serial.println(">");
  newData = false;
  askForData = true;
}
