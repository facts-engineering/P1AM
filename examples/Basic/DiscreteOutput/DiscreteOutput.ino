/*
	Example: DiscreteOutput

	This example shows how to write to discrete output modules. 
	Discrete ouputs are any output that is only ON or OFF such as a relay.

	This example works with all P1000 Series:
	 - Discrete ouput modules such as P1-08TRS, P1-08TD1, P1-16TR, etc.
	 - Discrete combo modules such as P1-15CDD1, P1-15CDD2, P1-16CDR, etc.  

	This example will toggle channel 2 of slot 1 on and off every second.
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

void loop(){  // the loop routine runs over and over again forever:

  P1.writeDiscrete(HIGH,1,2);  //Turn slot 1 channel 2 on
  delay(1000);  //wait 1 second
  P1.writeDiscrete(LOW,1,2);   //Turn slot 1 channel 2 off
  delay(1000);  //wait 1 second
  
}
