/*
  These functions are not recommended for new programmers who are unfamiliar with using arrays or the 
  P1000 I/O modules. These functions do not prevent you from writing incorrectly formatted data. You 
  should have a good understanding of the bitmapped writeDiscrete function before using these functions.
---------------------------------------------------------------------------------------------------------
  
  Example: BlockDataTransfer  
  This example shows how to use the readBlockData and writeBlockData functions to communicate
  to P1000 Series Modules. Block data transfers have a speed advantage over individual channel and discrete 
  bitmapped channel transfers when communicating to many modules or channels in the base. Since all data is 
  transferred at once, you eliminate time waiting for data to propogate in individual function calls.
    
  Base Controller Data Structures:
  The base controller (BC) is the microcontroller that is responsible for managing all of the data and modules
  connected in the system. The BC is the middle-man between the Atmel processor and the Modules in the base.

  Each P1000 module reserves a certain number of bytes of each type of data during the init procedure. The values
  for each module and each type can be referenced in the Module_List.h file that is included in this library.
  The BC contains 5 data arrays that are 1200 bytes long. Typically, each module reserves the following amount of each type. 
  
    1. Discrete Input Data     - 1 byte per 8 Discrete Input channels
    2. Discrete Output Data    - 1 byte per 8 Discrete Output channels
    3. Analog Input Data       - 4 bytes per Analog Input channel
    4. Analog Output Data      - 4 bytes per Analog Output channel
    5. Status register Data    - Varies, see Module_List.h

  Each byte of discrete data is treated as a bitmapped representation of the channels.
  Note: Modules with 15 channels still use the full 2 bytes for their data.  
  
  Each type of data for each module also has an offset in the BC. The offset is determined by the order and particular 
  selection of modules in the base setup. An base is shown below using a mix of discrete inputs and outputs.
  The offset of the data arrays for each module is in each box.
   _____   _______   _______   _______   _______   _______   _______   _______   _______     
  |  P1 | |   0   | |   0   | |   1   | |   0   | |   0   | |   0   | |   1   | |   0   |
  |  AM | |   8   | |   8   | |   6   | |   8   | |   8   | |   8   | |   6   | |   8   |  
  |  -  | |   T   | |   T   | |   T   | |   T   | |   N   | |   T   | |   N   | |   S   |  
  |  C  | |   R   | |   R   | |   R   | |   R   | |   D   | |   R   | |   D   | |   I   |  
  |  P  | |   S   | |   S   | |       | |   S   | |   3   | |   S   | |   3   | |   M   |  
  |  U  | |DO - 0 | |DO - 1 | |DO - 2 | |DO - 4 | |DI - 0 | |DO - 5 | |DI - 1 | |DI - 3 | 
   ¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯ 
                                    
  Each analog channel uses 4 bytes of data, regardless of the resolution of the module. For Voltage/Current analog
  modules, this data uses the big endian byte order. This means the "Most Significant Byte" (MSB) of the data is placed 
  at the byte with the lowest address. For temperature modules the data is  simply encoded as a 32-bit float. This is 
  covered more thoroughly in the BlockDataTransferAnalog example. Enabling/Disabling channels or otherwise changing 
  configurations does not affect the amount of data reserved by a module. 
  
  
  This example will use up to 15 discrete output modules. The modules can be a mix of either 8 or 16 channels.
  Every 8 channels will be considered a group and treated like a 8-bit counter. Every second the counter will 
  increment the outputs and print the Hexadecimal representation to the serial monitor. Modules with 8 channels
  will all have the same outputs. Modules with 16 channels will have 2 sets of values that are the same as the 
  8 channel modules. It will then read all of the Discrete Outputs bytes written to the BC and print those to 
  the Serial Monitor.
  
  
  This example works with all P1000 Series Discrete Output Modules, but can be adapted to any type of module.
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

char dataArrayOut[30];    //Memory for up to 15 discrete outputs modules with up to 16 channels
char dataArrayIn[30];   //Memory to read back the values we wrote to the modules
char eightBitCounter = 0; //Value for our "Counter" that will control the channel output states

void loop(){  // the loop routine runs over and over again forever:


  for(int i = 0; i < 30; i++){
    dataArrayOut[i] = eightBitCounter;    //Set our output array to our counter value
  }

  P1.writeBlockData(dataArrayOut,30,0,DISCRETE_OUT_BLOCK);  //Write all 30 bytes of dataArrayOut to the BC Discrete Output Data array
  P1.readBlockData(dataArrayIn,30,0,DISCRETE_OUT_BLOCK);  //Read back the 30 bytes just sent to the BC to dataArrayIn

  //Show the value of the counter
  Serial.print("Counter Hex Value =  ");
  Serial.println(eightBitCounter,HEX);

  //Print out each of the bytes just read in. These should match up the dataArrayOut values we just sent
  for(int i = 0; i < 30; i++){
    Serial.print("Discrete Output Byte ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(dataArrayIn[i],HEX);
  }

  eightBitCounter++; //Increment counter

  Serial.println("");
  delay(1000);

}
