/*
  Example: ReadStatus

  This example shows how to read the status information from various 
  P1000 Series modules. This status information includes things like 
  "Missing 24V",Over and Under range indiciation, and more. See online
  documentation for the information provided by each module 

  This example works with all P1000 Series Modules that provide a 
  "Missing 24V" status indication. It can be easily adapted to read other
  information as well. 

  This example will monitor the Missing 24V bit. This is present on most 
  modules with a field 24V input. The bit will go high when 24V is no 
  longer present on the terminal. Two different methods are used to read
  this status bit. The first uses the check24V function which simply returns 
  the missing 24V bit for a slot. The second option uses an array and returns all
  status bytes for the module.
  
  Missing 24V is bit 1 of status byte 3 in most modules. Bit and Byte numbering 
  begins at "0". Other similar functions are commented out below. These can be
  used with modules that support these statuses.
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

void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

char statusByteArray[12]; //Array for second implementation where all bytes are read
bool statusBit;     //Variable to temporarily store individual bits of our Status Bytes

void loop(){  // the loop routine runs over and over again forever:
  
  /*************************/
  /*First Option - Simple*/
  /*************************/
  
  statusBit = P1.check24V(1);	//Read the missing 24V bit on slot 1.
//bool underRangeBit = checkUnderRange(1,3) //Check under range bit on slot 1 channel 3. 
//bool overRangeBit = checkOverRange(1,3) //Check over range bit on slot 1 channel 3. 
//bool burnoutBit = checkBurnout(1,3) //Check under range bit on slot 1 channel 3.  
 
  Serial.print("Check 24V function: ");
  if(statusBit){  //Check if bit position 1 is on. This indicates Missing 24V.
    Serial.println("Missing 24V");
  }
  else{
    Serial.println("24V Present");
  }
  
  /*************************/
  /*Second Option - One Byte*/
  /*************************/
  P1.readStatus(statusByteArray,1); //Store all Status Bytes of slot 1 into statusByteArray
  
  Serial.print("All byte check: ");
  if(statusByteArray[3] & 0b10){  //Check if bit position 1 of Status byte 3 is on. This indicates Missing 24V.
    Serial.println("Missing 24V");
  }
  else{
    Serial.println("24V Present");
  }

  Serial.println(""); //Blank line for readability
  delay(1000);  //Wait 1 second
}
