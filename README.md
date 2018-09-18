# Data Logger (and using cool sensors!)


## Part A.  Writing to the Serial Monitor
 
**a. Based on the readings from the serial monitor, what is the range of the analog values being read?**
 
 0-1023
 
**b. How many bits of resolution does the analog to digital converter (ADC) on the Arduino have?**

2^10 = 1024

## Part B. RGB LED

**How might you use this with only the parts in your kit? Show us your solution.**

![led](https://github.com/Yiyuan7/IDD-Fa18-Lab3/blob/master/IMG_1620.JPG)

## Part C. Voltage Varying Sensors 
 
### 1. FSR, Flex Sensor, Photo cell, Softpot

**a. What voltage values do you see from your force sensor?**

230,231

**b. What kind of relationship does the voltage have as a function of the force applied? (e.g., linear?)**

The More pressure, the higher the voltage but not strictly linear.

**c. Can you change the LED fading code values so that you get the full range of output voltages from the LED when using your FSR?**

Yes

**d. What resistance do you need to have in series to get a reasonable range of voltages from each sensor?**

200ohm

**e. What kind of relationship does the resistance have as a function of stimulus? (e.g., linear?)**

It's linear

### 2. Accelerometer
 
**a. Include your accelerometer read-out code in your write-up.**



### 3. IR Proximity Sensor

**a. Describe the voltage change over the sensing range of the sensor. A sketch of voltage vs. distance would work also. Does it match up with what you expect from the datasheet?**

When I go closer it will increase and when I leave it will drop. 
Yes, it match.

**b. Upload your merged code to your lab report repository and link to it here.**



## Optional. Graphic Display

**Take a picture of your screen working insert it here!**

## Part D. Logging values to the EEPROM and reading them back
 
### 1. Reading and writing values to the Arduino EEPROM

**a. Does it matter what actions are assigned to which state? Why?**

yes, because if you take write s first step, clen up as secong, show on screen as third step, then we cna see nothing at this time.

**b. Why is the code here all in the setup() functions and not in the loop() functions?**

In the main function, the code lies in loop(). It can call those stat0/state1/state2's code in setup()

**c. How many byte-sized data samples can you store on the Atmega328?**

1024 bytes

**d. How would you get analog data from the Arduino analog pins to be byte-sized? How about analog data from the I2C devices?**

set `analogReadResolution(8)` before `analogRead()`   

**e. Alternately, how would we store the data if it were bigger than a byte? (hint: take a look at the [EEPROMPut](https://www.arduino.cc/en/Reference/EEPROMPut) example)**

everytime we store a n-bit data, we move the address pointer forward by n.

**Upload your modified code that takes in analog values from your sensors and prints them back out to the Arduino Serial Monitor.**

int val = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  val = analogRead(A1);

  Serial.println(val);
}

### 2. Design your logger
 
**a. Insert here a copy of your final state diagram.**

![diagram](https://github.com/Yiyuan7/IDD-Fa18-Lab3/blob/master/IMG_1663.JPG)

### 3. Create your data logger!
 
**a. Record and upload a short demo video of your logger in action.**
