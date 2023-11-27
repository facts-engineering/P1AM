/*
  These functions are not recommended for new programmers who are unfamiliar with using arrays or the 
  P1000 I/O modules. These functions do not prevent you from writing incorrectly formatted data. You 
  should have a good understanding of the bitmapped writeDiscrete function before using these functions.
---------------------------------------------------------------------------------------------------------
  
  Example: BlockDataTransferAnalog  
  This example shows how to use the readBlockData and writeBlockData functions specifically with analog 
  P1000 series modules. It is largely the same as the BlockDataTransfer example but accounts for the
  endianess of analog modules. We recommend getting familiar with that example before using this one.
    
	
  Each analog channel uses 4 bytes of data, regardless of the resolution of the module. For Voltage/Current analog
  modules, this data uses the big endian byte order. This means that the 4th byte of each channel contains the
  least significant byte and the 1st byte contains the most significant byte. For temperature modules, the data is
  simply encoded as a 32-bit float regardless of whether a temperature or other range is used. Enabling/Disabling 
  channels or otherwise changing configurations does not affect the amount of data reserved by a module. 
  
  A base is shown below using a mix of analog inputs and outputs. The offset of the data arrays for each module 
  is in each box. Each analog channel uses 4 bytes. 
   _____   _______   _______   _______   _______  _______    
  |  P1 | |   0   | |   0   | |   0   | |   0   ||   0   | 
  |  AM | |   4   | |   8   | |   4   | |   4   ||   4   | 
  |  -  | |   A   | |   D   | |   A   | |   T   ||   D   | 
  |  C  | |   D   | |   A   | |   D   | |   H   ||   A   | 
  |  P  | |   L   | |   L   | |       | |   M   ||   L   | 
  |  U  | |AI - 0 | |AO - 0 | |AI - 16| |AI -32 ||AO - 32| 
   ¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯  ¯¯¯¯¯¯¯  
  
  
  
  This example will set the first 8 analog output channels in the base to 0x7FF and read the first 12 analog 
  input channels. It assumes the first 8 analog inputs channels are not from temperature modules
  and the 9th-12th channels are temperature measurements.

  
  This example works with all P1000 Series Analog Modules, but can be adapted to any type of module.
  
  Written by FACTS Engineering
  Copyright (c) 2023 FACTS Engineering, LLC
  Licensed under the MIT license.
*/
#include <P1AM.h>

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  
  while(!Serial){
	  ;	//Wait for open serial monitor
  }

  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }

}

char analogOutData[32];    //Raw Data for analog output array
char analogOutCheck[32];   //Array to read back and check values sent match	

char rawAnalogIn[32];   		//Array to store unformatted analog input bytes
int  analogChannelReadings[8];	//Array for individual channel data

char  littleEndianTemp[16];			//Array to store unformatted little endian temperature Bytes
char  bigEndianTemp[16];		    //Array to store unformatted big endian temperature Bytes
float temperatureChannelReadings[4];//Array for individual channel data properly formatted

void loop(){  // the loop routine runs over and over again forever:

  /*Analog Output Data*/
  for(int i = 0; i < 8; i++){     //Copy 0x000007FF into each channel with a big endian byte order
  	analogOutData[4*i + 0] = 0x00;  
	  analogOutData[4*i + 1] = 0x00;  
	  analogOutData[4*i + 2] = 0x07;  	
    analogOutData[4*i + 3] = 0xFF;  
  }

  Serial.println("Analog output data");
  P1.writeBlockData(analogOutData,32,0,ANALOG_OUT_BLOCK);  //Write all 32 bytes of dataArrayOut to the BC Discrete Output Data array
  P1.readBlockData(analogOutCheck,32,0,ANALOG_OUT_BLOCK);  //Read back the 32 bytes just sent to the BC to dataArrayIn

  //Print out each of the bytes just read in. These should match up the dataArrayOut values we just sent
  for(int i = 0; i < 32; i++){
    Serial.print("Analog Output Byte ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(analogOutCheck[i],HEX);
  }
  Serial.println("");
  delay(5000);	//Wait until we do the next section
   
   
   /*Analog Input Data*/
   P1.readBlockData(rawAnalogIn,32,0,ANALOG_IN_BLOCK);  //Read first 32 bytes (8 channels) of analog input data
   
  for(int i = 0; i < 8; i++){
	  analogChannelReadings[i] = rawAnalogIn[4*i + 0]<<24;   //OR and shift MSB
	  analogChannelReadings[i] |= rawAnalogIn[4*i + 1]<<16;   //OR and shift 3rd byte
	  analogChannelReadings[i] |= rawAnalogIn[4*i + 2]<<8; 	  //OR and shift 2nd byte
    analogChannelReadings[i] |= rawAnalogIn[4*i + 3];       //OR LSB
  }

  //Print out formatted data for each channel
  for(int i = 0; i < 8; i++){
    Serial.print("Analog Input Channel ");
    Serial.print(i+1);
    Serial.print(" Hex Counts = ");
    Serial.println(analogChannelReadings[i],HEX);
  }
  
  Serial.println("");
  delay(5000);	//Wait 5 seconds before we do the next section

  
  /*Temperature Input Data*/
  P1.readBlockData(littleEndianTemp,16,32,ANALOG_IN_BLOCK); // Read 16 bytes (4 channels) of analog data starting at offset 32 (9th analog in channel in base)

  //Reverse byte order to correct for endianess
  for(int i = 0; i < 4; i++){
    bigEndianTemp[4*i + 3] = littleEndianTemp[4*i + 0];   
    bigEndianTemp[4*i + 2] = littleEndianTemp[4*i + 1];  
    bigEndianTemp[4*i + 1] = littleEndianTemp[4*i + 2];  
    bigEndianTemp[4*i + 0] = littleEndianTemp[4*i + 3];  
  }

  memcpy(temperatureChannelReadings,bigEndianTemp,16); //Copy 16 bytes into our float array. Floats are 4 bytes each.

  for(int i = 0; i < 4; i++){
    Serial.print("Temperature Input Channel ");
    Serial.print(i+1);
    Serial.print(" = ");
    Serial.print(temperatureChannelReadings[i],2);
	  Serial.println(" degrees");
  }

  Serial.println("");
  delay(60000);	//Wait 1 minute before re-running
}
