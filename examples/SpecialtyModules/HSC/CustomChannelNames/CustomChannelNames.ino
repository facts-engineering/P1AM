/*
  Example: CustomChannelNames

  This example shows how to create your own instances of the P1_HSC_Channel class for more readable code 
  for your application

  This example works with the P1-02HSC
 
  This example will create 2 objects of the P1_HSC_Channel class, manually set the position of each of them and then
  read that position back and print it to the serial monitor.
    
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

P1_HSC_Module packagingLine(1); //Create HSC class object for slot 1. 
P1_HSC_Channel conveyorA(1,1);  // Conveyor encoder connected to Slot 1 Channel 1 of the HSC module
P1_HSC_Channel armX(1,2);       // Arm encoder connected to Slot 1 Channel 2 of the HSC module

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!Serial){
    ; //Wait for Serial Port to be opened
  }
  
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
  
  conveyorA.isRotary = false;
  conveyorA.enableZReset = false;
  conveyorA.inhibitOn = false; //oneZ, threeIn, twoZ, fourIn
  conveyorA.mode = stepDirection;  //quad4x, quad1x
  conveyorA.polarity = positiveDirection;  //negativeDirection
  
  armX.isRotary = false;
  armX.enableZReset = false;
  armX.inhibitOn = false; //oneZ, threeIn, twoZ, fourIn
  armX.mode = stepDirection;  //quad4x, quad1x
  armX.polarity = positiveDirection;  //negativeDirection
  
  packagingLine.configureChannels(conveyorA,armX);  //Load settings into HSC module. 2 channel class object must be passed in, 1 for each channel. 

  conveyorA.setPosition(1000); //Initialise positions
  armX.setPosition(2000);
  
  delay(100);
}

int channelOneCounts = 0;
int channelTwoCounts = 0;

void loop(){  // the loop routine runs over and over again forever:

 channelOneCounts = conveyorA.readPosition();
 channelTwoCounts = armX.readPosition();

 Serial.print("Conveyor A Position = ");
 Serial.println(channelOneCounts);
 Serial.print("Arm X Position = ");
 Serial.println(channelTwoCounts);

 delay(1000);  
}