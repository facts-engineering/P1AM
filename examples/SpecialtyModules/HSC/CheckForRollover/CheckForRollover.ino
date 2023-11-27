/*
  Example: CheckForRollover

  This example shows how to use the readRollOver function. This function should be used in a polling style
  as the flag to indiciate rollover is only up for a short amount of time.

  This example works with the P1-02HSC
 
  This example will check to see if the first channel of the HSC modules has rolled over or under and post the
  current rollover count to the serial monitor. The rollover point is set to 1000.
    
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
#include <P1_HSC.h>

P1_HSC_Module HSC(1); //Create HSC class object for slot 1. It also automatically creates 2 P1_HSC_CHANNEL objects for this slot

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!Serial){
    ; //Wait for Serial Port to be opened
  }
  
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
  
  /*
  Comments in the lines below shows other settings
  -isRotary - If true configures channel as a rotary encoder. 
			  This means it will roll over at the rollover position.
  -enableZReset - If true, resets current position when channel's Z input is high.
  -inhibitOn - When the selected input is high, counting is inhibited. "false" 
			   disables this feature.
  -mode - Select counting mode: Step and Direction, Quadrature 4X, and Quadrature 1X.
  -polarity - Sets directional polarity of counting.
  
  */
  HSC.CNT1.isRotary = true;
  HSC.CNT1.enableZReset = false;
  HSC.CNT1.inhibitOn = false; //oneZ, threeIn, twoZ, fourIn
  HSC.CNT1.mode = stepDirection;  //quad4x, quad1x
  HSC.CNT1.polarity = positiveDirection;  //negativeDirection

  HSC.configureChannels();  //Load settings into HSC module. Leave argument empty to use default CNT1 and CNT2 
  
  HSC.CNT1.setRolloverPosition(1000); //Initialise positions
  
  delay(100);
}

int rolloverCounter = 0;
int rolloverFlag = 0;

void loop(){  // the loop routine runs over and over again forever:

  rolloverFlag = HSC.CNT1.readRollOver(); //1 if rolled over, -1 if rolled under, 0 for no rollover set.

  if(rolloverFlag != 0){  //If rolled Over or Under
    rolloverCounter += rolloverFlag;  //Add flag value to the rollover counter.
    Serial.println(rolloverCounter);
  }
}
