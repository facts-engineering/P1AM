/*
  Example: Z_Reset

  This example shows how to use the Z reset functionality of the HSC module. When this is enabled,
  if the Z input goes high the current position of the channel will reset to the defined Z reset position.

  This example works with the P1-02HSC.
 
  This example will set the Z reset position and then print each channels current position to the serial monitor.
  Once the appropriate Z input goes high, the counts will be set to the Z Reset Value.
    
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
  
  HSC.CNT1.isRotary = false;
  HSC.CNT1.enableZReset = true;
  HSC.CNT1.inhibitOn = false; //oneZ, threeIn, twoZ, fourIn
  HSC.CNT1.mode = stepDirection;  //quad4x, quad1x
  HSC.CNT1.polarity = positiveDirection;  //negativeDirection
  
  HSC.CNT2.isRotary = false;
  HSC.CNT2.enableZReset = true;
  HSC.CNT2.inhibitOn = false; //oneZ, threeIn, twoZ, fourIn
  HSC.CNT2.mode = stepDirection;  //quad4x, quad1x
  HSC.CNT2.polarity = positiveDirection;  //negativeDirection
  
  HSC.configureChannels();  //Load settings into HSC module. Leave argument empty to use default CNT1 and CNT2 

  HSC.CNT1.setPosition(0); //Initialise positions
  HSC.CNT2.setPosition(0);
  
  HSC.CNT1.setZResetValue(15000);  //Sets the position to reset to if the Z reset pin is triggered
  HSC.CNT2.setZResetValue(7777);
  
  delay(100);
}

int channelOneCounts = 0;
int channelTwoCounts = 0;

void loop(){  // the loop routine runs over and over again forever:

 channelOneCounts = HSC.CNT1.readPosition();
 channelTwoCounts = HSC.CNT2.readPosition();

 Serial.print("Channel 1 Position = ");
 Serial.println(channelOneCounts);
 Serial.print("Channel 2 Position = ");
 Serial.println(channelTwoCounts);

 delay(1000);  
}