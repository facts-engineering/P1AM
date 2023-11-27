/*
  Example: START2_PUZZLE

  This example is a puzzle game using the P1-08TRS and P1-08SIM. At the start and secret number is
  generated that represents what each P1-08SIM switch should be set to. The P1-08TRS will show
  the player how many switches are currently correct - but not which ones.

  The game begins by flipping the toggle switch. The player must solve the puzzle as quickly as possible.
  At the end of the game the total time will be shown.

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


bool switchStart;
byte secretNumber = 0;
unsigned long timeStart;
unsigned long timeEnd;
float timeElapsed;

void setup(){
 
  pinMode(SWITCH_BUILTIN,INPUT);
  switchStart = digitalRead(SWITCH_BUILTIN);
  randomSeed(analogRead(0));

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

void loop() {
  
  waitForStart(); //Wait for switch to change position
  
  secretNumber = random(256);     //Get our puzzle number
  byte switches = P1.readDiscrete(2);
  while(switches == secretNumber){
    secretNumber = random(256); //Just in case we got lucky, let's get a new number
  }
  
  timeStart = millis(); //Time start
  timeEnd = playGame(secretNumber); //Loops until the game is won
  timeElapsed = (float)(timeEnd - timeStart)/1000;  //Get elapsed time in seconds
    
  Serial.println("Final Time is: " + (String)(timeElapsed) + " seconds");
  Serial.println("Toggle Switch to play again");
  
  for(int i = 0; i < 8; i++){ //Simple Visual to indicate end of game
    P1.writeDiscrete(1<<i,1); // Only turn on one output at a time - Use shifts to decide which
    delay(200);
  }
  P1.writeDiscrete(0,1);  //Clear TRS at end of game
  
}
unsigned long playGame(byte secretNum){
  byte switches;
  int numberRight = 0;
  byte outputValue = 0;

  //Keep checking until our switches match our secret number
  while(switches != secretNum){ 
    switches =  P1.readDiscrete(2);
    numberRight = checkScore(secretNum, switches); 
    P1.writeDiscrete(numberRight, 1);  
  }

  return millis();
}


byte checkScore(byte goal, byte current){
  byte n = 0;
  byte matching = 0;
  byte score = 0;
  
  n = ~(goal ^ current); //Sets all matching bits. XNOR logic gate.
  
  while(n){  //Count Correct
    matching += n & 1;  //Add 1 if matching
    n>>=1; // Shift to next bit
  }

  for(int i = 0; i < matching; i++){  //Convert number right to 1 channel on for each correct
    score<<=1;  
    score++;
  }
  
  return score;
}

void waitForStart(){

  Serial.println("Toggle Switch to play");
  while(digitalRead(SWITCH_BUILTIN) == switchStart);
  switchStart = !switchStart;
}
