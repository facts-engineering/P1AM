/*
	Example: AnalogOutput

	This example shows how to write to analog output modules. 
	Analog ouputs generate a voltage or current signal.
    Ranges can be between 0-10V, 4-20mA, etc.

	This example works with all P1000 Series:
	 - Analog ouput modules such as P1-04DAL-1, P1-04DAL-2,etc.
	 - Analog combo modules such as P1-4ADL2DAL-1, P1-4ADL2DAL-2 etc.  

	This example will ramp up the output of slot 1, channel 2 every second. It resets at 4095 for a 12 bit module. 
	If you are using a different resolution module adjust accordingly.
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

 To confirm the analog output value you can use either a P1 analog input module or a multimeter.
 
 Written by FACTS Engineering
 Copyright (c) 2023 FACTS Engineering, LLC
 Licensed under the MIT license.
*/

#include <P1AM.h>

int outputCounts = 0;    //variable outputCounts is the output value of slot 1, channel 2.

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

void loop(){  // the loop routine runs over and over again forever:

  P1.writeAnalog(outputCounts, 1, 2); //writes analog data to P1 output module
  outputCounts = outputCounts + 100;  //increments output counts by 100

  if(outputCounts > 4095){  //resets output to 0 when it reaches max value for a 12 bit module
    outputCounts = 0;
  }
  delay(1000); //1 seconds delay
}
