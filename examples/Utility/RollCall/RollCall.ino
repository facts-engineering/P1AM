/*
	Example: RollCall

	This example shows how to use the rollCall function to ensure that
	modules present in the base match a pre-defined list. P1.rollCall should
	be called immediately after P1.init for best results.

	This example works with all P1000 Series Modules
 

	This example will check to see if Slot 1 is a P1-08TRS and Slot 2 
	is a P1-08SIM. If either module is missing or incorrect the P1AM-CPU will:
		1. Report the error to the serial monitor.
		2. Disable any further writes or reads to or from the module.
		3. Return a bitmapped representation of any module in error
		   i.e. an error in Slots 1 and 3 would return 0x5. No errors
		   found would return 0.

		
	 _____  _____  _____
	|  P  ||  S  ||  S  |
	|  1  ||  L  ||  L  |
	|  A  ||  O  ||  O  |
	|  M  ||  T  ||  T  |
	|  -  ||     ||     |
	|  C  ||  0  ||  0  |
	|  P  ||  1  ||  2  |
	|  U  ||     ||     |
	 ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯

	Written by FACTS Engineering
	Copyright (c) 2023 FACTS Engineering, LLC
	Licensed under the MIT license.
*/

#include <P1AM.h>

const char* baseArray[] = {"P1-08TRS","P1-08SIM"};	//Modules expects in base. They should be written in order from left to right.

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!Serial){
    ; //Wait for Serial Port to be opened
  }
  
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
  
  
  int baseErrors = P1.rollCall(baseArray,2);	//Pass in list of modules and the number to check
  
  if(baseErrors != 0){	//If any errors were found
	  
  	while(true){//This is not the expected lineup - Stay in this loop so none of the control program is executed.
  	  Serial.println(baseErrors,BIN);		//Prints binary representation of error
		delay(1000);		  
  	}
  }
}

void loop(){  // the loop routine runs over and over again forever:

  P1.writeDiscrete(HIGH,1,2);  //Turn slot 1 channel 2 on
  delay(1000);  //wait 1 second
  P1.writeDiscrete(LOW,1,2);   //Turn slot 1 channel 2 off
  delay(1000);  //wait 1 second
  
}