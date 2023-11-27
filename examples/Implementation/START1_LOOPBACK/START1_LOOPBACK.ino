/*
  Example: START1_LOOPBACK

  This example shows how to translate counts to mA for the input and output
  of the P1-4ADL2DAL-1 in the start kit. 

  13 bit resolution input, range is 0-20mA so 0mA is 0 and 20mA is 8191 in decimal
  12 bit resolution output, range is 4-20mA so 4mA is 0 and 20mA is 4095 in decimal
  This example requires you to wire channel 1 of the output to channel 1 of the input.

  This example is written for the P1AM-START1, but can be easily adapted for
  other module lineups. Slot 1 is the P1-4ADL2DAL-1 module. 
    
   _____  _____
  |  P  ||  P  |
  |  1  ||  1  |
  |  A  ||  -  |
  |  M  ||  C  |
  |  -  ||  O  |
  |  C  ||  M  |
  |  P  ||  B  |
  |  U  ||  O  |
  |     ||     |
   ¯¯¯¯¯  ¯¯¯¯¯ 

  Written by FACTS Engineering
  Copyright (c) 2023 FACTS Engineering, LLC
  Licensed under the MIT license.
*/

#include <P1AM.h>

void setup() {
  Serial.begin(115200);     //start serial monitor

  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

int inputCounts = 0;                          //declare input counts variable
int outputCounts = 0;                         //start output counts at 0
float inputAmps = 0;                            //declare input milliamps variable
float outputAmps = 0;                           //declare output milliamps variable

void loop() {
  P1.writeAnalog(outputCounts, 1, 1);         //output from slot 1 channel 1
  inputCounts = P1.readAnalog(1, 1);          //read input of slot 1 channel 1  
  Serial.print("Input is ");
  Serial.print(inputCounts);                //prints input counts to serial monitor
  Serial.println(" counts.");
  Serial.print("Output is ");
  Serial.print(outputCounts);               //prints input counts to serial monitor
  Serial.println(" counts.");

  inputAmps = convert13Bit(inputCounts);      //convert input to milliamps
  outputAmps = convert12Bit(outputCounts);    //convert output to milliamps

  Serial.print("Input is ");
  Serial.print(inputAmps);                  //prints input counts to serial monitor
  Serial.println(" milliamps.");
  Serial.print("Output is ");
  Serial.print(outputAmps);                 //prints input counts to serial monitor
  Serial.println(" milliamps.");

  outputCounts += 1;
  if(inputCounts > 8191){                     //ramp up the input current until it reaches 8191 counts (20mA)
    outputCounts = 0;
  }
  delay(100);                                  //100 millisecond delay
}

float convert12Bit(int counts){
  float milliamps = 16*((float)counts/4095);    //Math to convert 12 bit counts to milliamps
  milliamps += 4;                               //output range is 4-20mA, 0 counts is 4mA so 4 is added as an offset
  return milliamps;
}

float convert13Bit(int counts){
  float milliamps = 20*((float)counts/8191);    //Math to convert 13 bit counts to milliamps
  return milliamps;
}
