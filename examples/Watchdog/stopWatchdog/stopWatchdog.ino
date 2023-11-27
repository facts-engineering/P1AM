/*  
    stopWatchdog example

    The watchdog timer is a safety feature that will stop code from
    executing if the P1AM-CPU is not operating properly. It is used to detect
    and recover from potential hardware or programming faults.

    This example shows how to initialize and then stop the watchdog timer. 
    Stopping the watchdog may be necessary depending

    This example requires any P1000 series module.
	
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

void setup() {
  Serial.begin(115200); //initialize serial communication at 115200 bits per second 
    while (!P1.init()) 
    {    
      ;//Wait for Modules to Sign on
    }
    /*
     *        It is required to call the configWD and startWD functions before using the watchdog timer.
     */
  P1.configWD(5000, TOGGLE); //Pass in timer value and set the Watch Dog to TOGGLE mode. Watch Dog timer value is in milliseconds. 
                             //In this example the Watchdog is set to TOGGLE the reset line and it will trigger after 5000 milliseconds
  P1.startWD();              //start Watch Dog function. Timer starts at 0 and waits the previously passed in value (5000 ms). 
}

int waitTime = 0;

void loop() {
  //The Watch Dog is reset when any module function is called.
  //alternatively the petWD function can be used
  P1.petWD(); //when called petWD will reset Watchdog timer to 0.

  P1.stopWD(); //when called stopWD will stop the Watchdog timer
  //can be used to allow a certain portion of code to execute without the fear that the arduino cpu will reset during execution.

  while(waitTime < 10){
    waitTime = waitTime + 1; //10 second loop.
    delay(1000);  //delay 1000 milliseconds
  }

  P1.startWD(); //watchdog can be restarted after the long delay
}
