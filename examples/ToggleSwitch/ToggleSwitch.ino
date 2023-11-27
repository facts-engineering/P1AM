/*
	Example: ToggleSwitch

	This example shows how to use the toggle switch and LED on the front of the P1AM-100

	This example does not require any modules

	This example will turn on the LED when the switch is up, and turn it off when the 
	switch is down
	 _____  
	|  P  |
	|  1  |
	|  A  |
	|  M  |
	|  -  |
	|  C  |
	|  P  |
	|  U  |
	 ¯¯¯¯¯  
	Written by FACTS Engineering
	Copyright (c) 2023 FACTS Engineering, LLC
	Licensed under the MIT license.
*/

#include <P1AM.h>

void setup(){ // the setup routine runs once:
  pinMode(LED_BUILTIN,OUTPUT);	//Set our LED (Pin 32) to be an output
  pinMode(SWITCH_BUILTIN,INPUT);//Set our Switch (Pin 31) to be an input
}

bool switchState = 0; //Variable to hold the current state of the switch

void loop(){  // the loop routine runs over and over again forever:
  
  switchState = digitalRead(SWITCH_BUILTIN);//Read the state of the switch
  digitalWrite(LED_BUILTIN,switchState);	//Update the LED
  
}
