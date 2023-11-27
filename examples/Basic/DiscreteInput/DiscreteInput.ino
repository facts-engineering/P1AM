/*
	Example: DiscreteInput

	This example shows how to read from discrete input modules. 
	Discrete intputs are any input that is only ON or OFF such as a proximity sensor.

	This example works with all P1000 Series:
	 - Discrete input modules such as P1-08SIM, P1-08ND3, P1-08NA, etc
	 - Discrete combo modules such as P1-15CDD1, P1-15CDD2, P1-16CDR, etc.  

	This example will use a discrete input in slot 1 and will print out the value of channel 2.
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

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

int channelTwo;
void loop(){  // the loop routine runs over and over again forever:
	
	channelTwo = P1.readDiscrete(1,2);	//Read the value of channel 2 in slot 1
	Serial.println(channelTwo);			//Print the value to the serial monitor. 1 is on, 0 is off
	delay(1000);						//Wait one second so it's easier to read the serial output.
  
}
