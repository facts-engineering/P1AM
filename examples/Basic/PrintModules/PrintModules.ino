/*
  Example: PrintModules

  This example shows how to manually print the list of modules present and the Base Controller firmware version.

  This example works with all P1000 Series modules

  This example will print out all modules that have signed on and the Base Controller firmware version. 
  Any amount of modules between 1 and 15 can be used with this example.
   _____  _____  _____  _____  _____  _____  _____  _____  _____  _____  _____  _____  _____  _____  _____  _____
	|  P  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  ||  S  |
	|  1  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  ||  L  |
	|  A  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  ||  O  |
	|  M  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  ||  T  |
	|  -  ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     |
	|  C  ||  0  ||  0  ||  0  ||  0  ||  0  ||  0  ||  0  ||  0  ||  0  ||  1  ||  1  ||  1  ||  1  ||  1  ||  1  |
	|  P  ||  1  ||  2  ||  3  ||  4  ||  5  ||  6  ||  7  ||  8  ||  9  ||  0  ||  1  ||  2  ||  3  ||  4  ||  5  |
	|  U  ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     ||     |
	 ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯
   	Written by FACTS Engineering
	Copyright (c) 2023 FACTS Engineering, LLC
	Licensed under the MIT license.
*/

#include <P1AM.h>

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

void loop(){  // the loop routine runs over and over again forever:
  
  P1.printModules();  //print out all currently signed-on modules to the console
  P1.getFwVersion();  //print the Base Controller's firmware version to the console
  Serial.println();   //print a blank line to look more organized
  delay(3000);        //wait three second so it's easier to read the serial output.
  
}
