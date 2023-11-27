/*
  Example: START1_LED

  This example changes the brightness of an LED based off of the input of the 
  P1-4ADL2DAL-1 in the start kit. You must provide a input current for the 
  module. 

  This example is written for the P1AM-START1, but can be easily adapted for
  other module lineups. Slot 1 is the P1-4ADL2DAL-1 module. 
    
   _____  _____  _____
  |  P  ||  P  ||  P  |
  |  1  ||  1  ||  1  |
  |  A  ||  A  ||  -  |
  |  M  ||  M  ||  C  |
  |  -  ||  -  ||  O  |
  |  G  ||  C  ||  M  |
  |  P  ||  P  ||  B  |
  |  I  ||  U  ||  O  |
  |  O  ||     ||     |
   ¯¯¯¯¯  ¯¯¯¯¯  ¯¯¯¯¯
  This example requires an LED (not provided in the start kit) attached to pin 6 of the P1AM-GPIO terminal block.

  Written by FACTS Engineering
  Copyright (c) 2023 FACTS Engineering, LLC
  Licensed under the MIT license.
*/

#include <P1AM.h>
 
const int ledPin = 6;                                //sets pin used for LED

void setup() {
  while(!P1.init()){
    ;                                                 //wait for base to initialize
  }
  Serial.begin(115200);                               //start serial monitor
}

int brightness = 0;                                   //variable to store the brightness value that will be written to the LED
int inputCounts = 0;                                  //variable to store the analog value read by the module

void loop() {  
  inputCounts = P1.readAnalog(1, 1);                  //reads the first channel of the P1-4ADL2DAL-1 in slot 1
  brightness = map(inputCounts, 0, 8191, 0, 255);     //map 13 bit inputCounts to 8 bit brightness
  analogWrite(ledPin, brightness);                    //writes brightness variable value to the LED
  Serial.println(brightness);                         //prints current LED brightness value to serial monitor 

  delay(15);                                          //delay 15 milliseconds
}
