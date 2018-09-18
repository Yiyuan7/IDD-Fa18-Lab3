
/ include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Basic demo for accelerometer readings from Adafruit LIS3DH
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include "Adafruit_VCNL4010.h"

// IR Proximity
Adafruit_VCNL4010 vcnl;
int value;

// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

void setup(void) {
  
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

  Serial.begin(9600);

  // Test connection of accelerometer LIS3DH
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) {   // i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");

  // setup VCNL4010
  Serial.println("VCNL4010 test");

  if (! vcnl.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.println("Found VCNL4010");


  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}


void loop() {
  lis.read();      // get X Y and Z data at once

  Serial.print("Ambient: "); Serial.println(vcnl.readAmbient());
  Serial.print("Proximity: "); Serial.println(vcnl.readProximity());
  
  // Then print out the raw data
  Serial.print("X:  "); Serial.print(lis.x); 
  Serial.print("  \tY:  "); Serial.print(lis.y); 
  Serial.print("  \tZ:  "); Serial.print(lis.z); 

  Serial.println();

  //print X Y and Z data on LCD display

  lcd.setCursor(0, 0);
  lcd.print(lis.x);
  lcd.setCursor(4, 0);
  lcd.print(',');
  lcd.setCursor(6, 0);
  lcd.print(lis.y);
  lcd.setCursor(10, 0);
  lcd.print(',');
  lcd.setCursor(12, 0);
  lcd.print(lis.z);
 
  delay(3000); 
}
