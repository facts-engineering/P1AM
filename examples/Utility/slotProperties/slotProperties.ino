/*
  Example: Slot Properties
  This example shows how to access the module properties at a given slot. 
  You can then use the module properties in your code without needing to know
  what is in the slot at compile time.  This example uses readBlockData to efficiently
  scan through the inputs for changes.

  This example works with all P1000 Series:
   - Discrete input modules such as P1-08SIM, P1-08ND3, P1-08NA, etc
   - Discrete combo modules such as P1-15CDD1, P1-15CDD2, P1-16CDR, etc. 

  This example will detect the change of any input and then print the byte, bit, and value.
   _____  _____  _____
  |  P  ||  S  ||  S  |
  |  1  ||  L  ||  L  |
  |  A  ||  O  ||  O  |
  |  M  ||  T  ||  T  |
  |  -  ||     ||     |
  |  C  ||  0  ||  0  |
  |  P  ||  1  ||  2  |
  |  U  ||     ||     | etc...
   ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯
  Written by Brett Bowden and Adam Cummick
  Licensed under the MIT license.
*/


#include <P1AM.h>

uint8_t digitalInputBytes = 0;
uint8_t digitalOutputBytes = 0;
uint8_t analogInputBytes = 0;
uint8_t analogOutputBytes = 0;

uint8_t dataInputArray[30] = {0};
uint8_t dataInputArray_old[30] = {0};

uint32_t checkInputsTimer = 0;

void setup(){

  Serial.begin(115200);
  while (!Serial) {}// wait for serial port to connect.

  uint8_t numberOfModules = 0;
  while(numberOfModules == 0) {
    numberOfModules = P1.init();
  }
  
  for (uint8_t slotIndex = 1; slotIndex <= numberOfModules; slotIndex++) {
    moduleProps slotProps = P1.readSlotProps(slotIndex);
    Serial.print("Slot: "); Serial.print(slotIndex); Serial.print(" Module: "); Serial.println(slotProps.moduleName);
    Serial.print("Digital Input Bytes: ");  Serial.println(slotProps.diBytes, DEC);
    Serial.print("Digital Output Bytes: "); Serial.println(slotProps.doBytes, DEC);
    Serial.print("Analog Input Bytes: ");   Serial.println(slotProps.aiBytes, DEC);
    Serial.print("Analog Output Bytes: ");  Serial.println(slotProps.aoBytes, DEC);
    Serial.println();

    digitalInputBytes  += (uint8_t)slotProps.diBytes;
    digitalOutputBytes += (uint8_t)slotProps.doBytes;
    analogInputBytes   += (uint8_t)slotProps.aiBytes;
    analogOutputBytes  += (uint8_t)slotProps.aoBytes;
  }

  Serial.print("Total Digital Input Bytes: ");  Serial.println(digitalInputBytes);
  Serial.print("Total Digital Output Bytes: "); Serial.println(digitalOutputBytes);
  Serial.print("Total Analog Input Bytes: ");   Serial.println(analogInputBytes);
  Serial.print("Total Analog Output Bytes: ");  Serial.println(analogOutputBytes);  

  P1.readBlockData((char*)dataInputArray, (uint16_t)digitalInputBytes, 0, DISCRETE_IN_BLOCK); // Read all inputs
  memcpy(dataInputArray_old, dataInputArray, sizeof(dataInputArray)); // Initialize with current readings
  
}

void loop(){
  if (millis() - checkInputsTimer > 10UL) { // Check inputs every 10ms
    P1.readBlockData((char*)dataInputArray, (uint16_t)digitalInputBytes, 0, DISCRETE_IN_BLOCK); // Read all inputs
    checkInputsTimer = millis();
    
    for (uint8_t i = 0; i < digitalInputBytes; i++) {
      if (dataInputArray[i] != dataInputArray_old[i]) {
        for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++) {
          if (bitRead(dataInputArray[i], bitIndex) != bitRead(dataInputArray_old[i], bitIndex)) {
            Serial.print("DI Byte: "); Serial.print(i); 
            Serial.print(" Bit: "); Serial.print(bitIndex); 
            Serial.print(" Changed to: "); Serial.println(bitRead(dataInputArray[i], bitIndex));
          }
        }
        dataInputArray_old[i] = dataInputArray[i];
      }
    }
  }
}
