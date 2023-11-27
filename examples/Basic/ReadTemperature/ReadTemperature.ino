/*
  Example: ReadTemperature
  
  This example shows how to read temperature values in degrees from a temperature input module. 
  The readTemperature function is also used when a temperature module is using a voltage or
  resistance range as it still returns a floating point value.
  
  Temperature inputs are analog inputs that read data from a temperature sensor and convert it into a 
  temperature reading. These include thermistors, thermocouples and resistance temperature detectors (RTDs) 
  
  This example works with all P1000 Series:
   - Temperature input modules such as P1-04NTC, P1-04RTD, and P1-04THM.
  
  The default ranges are:
   - P1-04THM: J-Type
   - P1-04RTD: Pt100
   - P1-04NTC: 2252
  
  This example will print channel 2's temperature to the serial monitor every second.
  This can be tested by:
   - Using an appropriate temperature probe for the module
   - Shorting the positive and negative inputs of a THM module to read room temperature.
   - Using a potentiometer to simulate thermistor/RTD signal on an NTC or RTD
      module and using a temp. vs. resistance chart to verify values. 
	  
     
  The reported temperature defaults to degrees Fahrenheit. 
	 _____  _____ 
	|  P  ||  S  |
	|  1  ||  L  |
	|  A  ||  O  |
	|  M  ||  T  |
	|  -  ||     |
	|  C  ||  0  |
	|  P  ||  1  |
	|  U  ||     |
	 ¯¯¯¯¯  ¯¯¯¯¯ 
	Written by FACTS Engineering
	Copyright (c) 2023 FACTS Engineering, LLC
	Licensed under the MIT license.
*/

#include <P1AM.h>

float temperature;  //Variable for the temperature being read. "float" variables can store decimal values like 1.23

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

void loop(){  // the loop routine runs over and over again forever:

  temperature = P1.readTemperature(1, 2);  //Return temperature read from channel 2 of the input module in slot 1
  
  Serial.print("Channel 2 Temperature: ");   
  Serial.println(temperature, 2);  //print the value in degrees Fahrenheit up to 2 decimal places
  
  delay(1000);  //wait 1 second
  
}
