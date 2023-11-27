/*
  Example: ReadInputs

  This example shows how to use the readInputs function for an HSC module

  This example works with the P1-02HSC
 
  This example will check the status of the P1-02HSC inputs and print them to the serial monitor
    
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
  HSC.CNT1.enableZReset = false;
  HSC.CNT1.inhibitOn = false; //oneZ, threeIn, twoZ, fourIn
  HSC.CNT1.mode = stepDirection;  //quad4x, quad1x
  HSC.CNT1.polarity = positiveDirection;  //negativeDirection
  
  HSC.CNT2.isRotary = false;
  HSC.CNT2.enableZReset = false;
  HSC.CNT2.inhibitOn = false; //oneZ, threeIn, twoZ, fourIn
  HSC.CNT2.mode = stepDirection;  //quad4x, quad1x
  HSC.CNT2.polarity = positiveDirection;  //negativeDirection
  
  HSC.configureChannels();  //Load settings into HSC module. Leave argument empty to use default CNT1 and CNT2 
  
  delay(100);
}

int allInputs = 0;

void loop(){  // the loop routine runs over and over again forever:

  allInputs = HSC.readInputs();
  Serial.print("All Inputs = ");
  Serial.println(allInputs,HEX);  //Print the status of each input as a hexadecimal number

  //HSC.readInputs also updates all of these
  Serial.print("1A = ");
  Serial.println(HSC.status1A);
  Serial.print("1B = ");
  Serial.println(HSC.status1B);
  Serial.print("1Z = ");
  Serial.println(HSC.status1Z);
  Serial.print("3IN = ");
  Serial.println(HSC.status3IN);
  Serial.print("2A = ");
  Serial.println(HSC.status2A);
  Serial.print("2B = ");
  Serial.println(HSC.status2B);
  Serial.print("2Z = ");
  Serial.println(HSC.status2Z);
  Serial.print("4IN = ");
  Serial.println(HSC.status4IN);

  delay(1000); 
}