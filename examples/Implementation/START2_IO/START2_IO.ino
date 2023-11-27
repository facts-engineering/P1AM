/*
  Example: START2_IO

  This example reads the state of each P1-08SIM input and writes it to the 
  corresponding P1-08TRS relay. It does this using 2 methods: Bitmapped and
  single point looping.

  This example is written for the P1AM-START2, but can be easily adapted for
  other module lineups. 
    
   _____  _____  _____
  |  P  ||  P  ||  P  |
  |  1  ||  1  ||  1  |
  |  A  ||  -  ||  -  |
  |  M  ||  0  ||  0  |
  |  -  ||  8  ||  8  |
  |  C  ||  T  ||  S  |
  |  P  ||  R  ||  I  |
  |  U  ||  S  ||  M  |
   ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯

  Written by FACTS Engineering
  Copyright (c) 2023 FACTS Engineering, LLC
  Licensed under the MIT license.
*/
#include <P1AM.h>

void setup(){

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

unsigned long startTime = 0;
unsigned long endTime = 0;

void loop() {

  startTime = millis();
  readAllChannels();  //Bitmapped
  endTime = millis();
  Serial.println("Bitmapped method time = " + (String)(endTime - startTime) + "ms");  //Subtract starting and ending time to get total
  
  startTime = millis();
  readForLoop();  //Single point loops
  endTime = millis();
  Serial.println("Loop method time = " + (String)(endTime - startTime) + "ms"); //Subtract starting and ending time to get total

  Serial.println();
  delay(100);  // Wait 1 second before starting next test  
}

/*Bitmapped method of reading and writing all the channels.
  Writing "0" for the channel arguments means we're addressing 
  all the channels for that slot. You can also simply omit the
  channel argument like this: P1.readDiscrete(2);
*/
void readAllChannels(){
  byte simValue = 0;

  simValue = P1.readDiscrete(2,0);
  P1.writeDiscrete(simValue,1,0);
  
}


//Looped method of reading and writing all channels
void readForLoop(){
  bool simValue = 0;
  
  for(int i = 1; i <= 8; i++){
    simValue = P1.readDiscrete(2,i);
    P1.writeDiscrete(simValue,1,i);
  }
}
