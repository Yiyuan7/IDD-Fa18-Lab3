#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

const int numStates = 3;
const int sensorMin = 1;
const int sensorMax = 25;
const int numberOfRotates = 24;
const int EEPROMSIZE= 1024;

const int SamplingRate = 10;
int currentWriteCount = 0;
int currentReadCount = 0;

int endAddr;

#define ENC_A 7 //these need to be digital input pins
#define ENC_B 6

int sensorPin = 0;    // select the input pin for the potentiometer
int ledPins[3] = {11, 10, 9}; // redPi=11   greenPin=10    bluePin=9

int state,lastState = -1;


void setup() {
  #ifndef ESP8266
    while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif
  
  Serial.begin(9600);  

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  for (int i = 0; i < 3; ++i) {
    pinMode(ledPins[i], OUTPUT);  
  }
  
  Serial.println("Setting up LIS3DH accelometer !");
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start LIS3DH accelometer");
    while (1);
  }
  
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  Serial.print("LIS3DH accelometer Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");

  setColor(255, 255, 255);
  doClearMemory();
}

void setColor(int red, int green, int blue)
{
  analogWrite(ledPins[0],  red);
  analogWrite(ledPins[1],  green);
  analogWrite(ledPins[2],  blue);  
}


void loop() {
  // map the controller range to number of states :
  static unsigned int prevCounter = 0;
  static unsigned int counter4x = 0; 
  static unsigned int counter = 0;
  
  static unsigned int encoderVal = 0;

  int tmpdata = read_encoder();
  if(tmpdata) {
    counter4x += tmpdata;
    counter = counter4x/4;
    if (prevCounter != counter){
      encoderVal = (prevCounter + 1) % numberOfRotates;
//      Serial.print("[Controller] encoder val = ");
//      Serial.println(encoderVal);
    }
    prevCounter = counter;
  }  

  lis.read();      // get X Y and Z data at once
  
  state = map(encoderVal, sensorMin, sensorMax, 0, numStates);
  int sensorValueX;
  int sensorValueY;
  int sensorValueZ;
  
  switch (state) {
  case 0:    
      if (lastState != state) {
         Serial.println("[State 0] doClearMemory");
         doClearMemory();
      }
    break;
  case 1:    
      if (lastState != state) {
         Serial.println("[State 1] doWriteToMemory");
      }
      currentWriteCount = (currentWriteCount + 1) % SamplingRate;
      if (currentWriteCount == 0) {
         sensorValueX = map(lis.x, -32768, 32767, 0, 255);
         sensorValueY = map(lis.y, -32768, 32767, 0, 255);
         sensorValueZ = map(lis.z, -32768, 32767, 0, 255);
         doWriteToMemory(sensorValueX, sensorValueY, sensorValueZ);
         Serial.println(sensorValueX);
         Serial.println(sensorValueY);
         Serial.println(sensorValueZ);
      }
    break;
  case 2:
      if (lastState != state) {
         Serial.println("[State 2] doReadFromMemory");
      }
       currentReadCount = (currentReadCount + 1) % SamplingRate;
       if (currentReadCount == 0) {
          doReadFromMemory();
       }
    break;
  } 
  lastState = state;
}


int read_encoder()
{
  static int enc_states[] = {
    0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0  };
  static byte ABab = 0;
  ABab *= 4;                   //shift the old values over 2 bits
  ABab = ABab%16;      //keeps only bits 0-3
  ABab += 2*digitalRead(ENC_A)+digitalRead(ENC_B); //adds enc_a and enc_b values to bits 1 and 0
  return ( enc_states[ABab] );
}


void doClearMemory() {
  Serial.println("[State 0] Clearing EEPROM");
    for (int i = 0; i < EEPROMSIZE; i++) {
    EEPROM.write(i, 0);
  }
  Serial.println("[State 0] EEPROM cleared");
}

void doWriteToMemory(int sensorValueX, int sensorValueY, int sensorValueZ ) {
//  Serial.println("[State 1] Writing sensor value to EEPROM");    
  int sensorArray[3] = {sensorValueX, sensorValueY, sensorValueZ};
  for (int i = 0; i < 3; i++) {
    EEPROM.write(endAddr, sensorArray[i]);
    endAddr = endAddr + 1;
    if (endAddr == EEPROMSIZE) {
      endAddr = 0;
    }
  }
//  Serial.println("[State 1] Sensorvalue stored to EEPROM!");
}

void doReadFromMemory() {
  byte value;
  for (int i = 0; i < EEPROMSIZE;) {
    byte red = EEPROM.read(i);
    byte green = EEPROM.read(i+1);
    byte blue = EEPROM.read(i+2);

    if (max(max(red, green), blue) == red) {
       setColor(0, 255, 255);
    } else if (max(max(red, green), blue) == green) {
       setColor(255, 0, 255);
    } else if (max(max(red, green), blue) == blue) {
       setColor(255, 255, 0);
    }
    
    delay(200);
    i = i + 3;
  }
}
