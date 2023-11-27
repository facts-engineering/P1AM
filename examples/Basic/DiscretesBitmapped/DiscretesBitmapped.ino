/*
	Example: DiscretesBitmapped

	This example shows how to read or write all channels of a discrete module in one command.
	This method is a faster option than addressing each channel individually. 

	This example works with all P1000 Series:
	 - Discrete ouput modules such as P1-08TRS, P1-08TD1, P1-16TR, etc.
	 - Discrete input modules such as P1-08SIM, P1-08ND3, P1-08NA, etc
	 - Discrete combo modules such as P1-15CDD1, P1-15CDD2, P1-16CDR, etc. 

	For this example the discrete output module is in slot 1 and the
	discrete input module is in slot 2. Combos can be used in either slot.	
	This example will toggle the first 8 discrete outputs in slot 1 every second 
	and will print the status of the first 8 discrete inputs to the serial monitor.
	  	 		
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
   
	Note: This example uses hexadecimal formatted numbers, though any number format
	is acceptable. So, anywhere 0xA5 is used, its decimal equivalent 165 can be substituted.
	
	To use this method, omit the channel argument in the discrete write/read functions.
		
		Normal:
		 P1.writeDiscrete(data,slot,channel);
		 val =  P1.readDiscrete(slot,channel);
		 
		 Bitmapped Method:
		 P1.writeDiscrete(data,slot);
		 val =  P1.readDiscrete(slot);
	
	Additionally, you can use the bitmapped method by passing "0" for the channel argument
	for either writeDiscrete() or readDiscrete():
	
		P1.writeDiscrete(0xFF,1,0);	//Turn on first 8 channels of slot 1
	
	When using this method, each channel is treated as a bit with channel 1 being 
	the Least Significant Bit or LSB. For example writing 0x73 to an 8 channel discrete 
	output would: Turn on channels 1, 2, 5, 6, and 7. Turn off channels 3,4, and 8. 
	When using a 16 channel module such as the P1-16TR, the number of digits used is doubled,
	e.g. writing 0x10FF would: Turn on channels 1-8 and 16. Turn off channels 9-15.
	These ideas work the same for discrete inputs, but with the value returned from the 
	readDiscrete function.

	Written by FACTS Engineering
	Copyright (c) 2023 FACTS Engineering, LLC
	Licensed under the MIT license.
*/

#include <P1AM.h>

int allInputChannels;	//Variable to store the reading from our discrete input

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

void loop(){  // the loop routine runs over and over again forever:

  //Toggle all 8 channels ever second
  P1.writeDiscrete(0xA5,1);  //Discrete output module. Data is bitmapped e.g. 1010 0101 or 0xA5 hex means channels 1,3,6, and 8 are ON. 2,4,5, and 7 are OFF.
  delay(1000);  //wait 1 second
  P1.writeDiscrete(0x5A,1);  //Discrete output module. Data is bitmapped e.g. 0101 1010 or 0x5A hex means channels 2,4,5, and 7 are ON. 1,3,6, and 8 are OFF.
  delay(1000);  //wait 1 second
  
  //Read all 8 input channels
  allInputChannels = P1.readDiscrete(2);	//Discrete input module. Data is bitmapped. If Channels 2 and 7 are on, this will return 0x42
  Serial.println(allInputChannels,HEX);		//Print value to serial monitor in hexadecimal format.
  
}


  
