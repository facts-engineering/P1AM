/*
	Example: AnalogInput

	This example shows how to read from analog input modules. 
	Analog inputs typically read a voltage or current signal.
  Ranges can be between 0-10V, 4-20mA, etc.

	This example works with all P1000 Series:
	 - Analog input modules such as P1-04ADL-1, P1-04ADL-2,etc.
	 - Analog combo modules such as P1-4ADL2DAL-1, P1-4ADL2DAL-2 etc.  

	This example will use a P1-04ADL-2 module in slot 1. It has a 13-bit resolution and reads a signal between 0-10V.
  We will use the built-in DAC on pin A0 as our output signal. It is 10-bit and outputs a signal in the range of 0-3.3V
  This example starts at outputting 1V and increases it by 0.1V every second. Once it hits 3.3V it resets to 0 and starts again.

  NOTE: While the built-in DAC is used in this example, you can use any appropriate source as a signal. If using a current input module, 
  DO NOT USE the built-in DAC.
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

int inputCounts = 0;    //Reading in Counts
float inputVolts = 0;   //Reading in Volts

int outputCounts = 0;   //Output value in Counts
float outputVolts = 1;  //Output value in Volts


void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
  analogWriteResolution(10);  //Set to use DAC at 10-bit resolution
}

void loop(){  // the loop routine runs over and over again forever:

  outputCounts = (outputVolts / 3.3) * 1023;	//Scale volts to 10-bit value
  analogWrite(A0, outputCounts);
  
  inputCounts = P1.readAnalog(1, 2); //Reads analog data from slot 1 channel 2 of the analog input module
  inputVolts = 10 * ((float)inputCounts / 8191);  //Convert 13-bit value to Volts
  
  Serial.println("Channel 2 is reading: ");
  Serial.print(inputVolts,2); //Print the value in Volts up to 2 decimal places
  Serial.println("V");
  Serial.print(inputCounts); //Print the value in counts
  Serial.println(" Counts");
  Serial.println();
  
  outputVolts = outputVolts + 0.01; //Increment the output value
  if(outputVolts > 3.3){  //Check if the max has been reached
    outputVolts = 0;
  }
  delay(1000);
}