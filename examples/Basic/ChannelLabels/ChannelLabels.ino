/*
	Example: ChannelLabels

	This example shows how to utilise "channelLabels" to help make your code more readable.

	This example works with all P1000 Series Modules..

	This example will use a discrete output in slot 1 and a discrete input in slot 2, but can easily be adapted to
	any type of input or output module.
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

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

channelLabel waterPump = {1,3};		//In our application, a water pump is controlled by channel 3 of our output module in slot 1 
channelLabel waterLevel = {2,4};	//We have a level indicator for our tank. If the tank hits a certain level, we need to start pumping.
bool levelReached = false;			//This is where we store our level limit reading

void loop(){  // the loop routine runs over and over again forever:
	
	while(levelReached == false){					//Loop until we hit the level we're waiting for
		levelReached = P1.readDiscrete(waterLevel);	//Keep checking if we've hit out level
	}
	
	P1.writeDiscrete(HIGH,waterPump);				//Start Draining
	while(levelReached == true){					//Loop until we're back to an acceptable level
		levelReached = P1.readDiscrete(waterLevel);	//Keep checking if we're back to a good level
	}
	P1.writeDiscrete(LOW,waterPump);				//Stop Draining
	
}
