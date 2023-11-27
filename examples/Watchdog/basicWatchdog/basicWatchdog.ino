/*  
    basicWatchdog Example
    The watchdog timer is a safety feature that will stop code from
    executing if the P1AM-CPU is not operating properly. It is used to detect
    and recover from potential hardware faults or programming mistakes.
    
    This example shows how to initialize and then use the watchdog. 
    There are 2 modes for the watchdog: Toggle and Hold. 
    - Toggle will reset the P1AM-CPU and then start code at the beginning.
    - Hold will hold the arduino in reset and shut off all modules. 
    
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

void setup() {  // the setup routine runs once:
  pinMode(SWITCH_BUILTIN, INPUT); //sets the switch on the faceplate as an input
  pinMode(LED_BUILTIN, OUTPUT);   //sets the user led to be an output
  
  Serial.begin(115200); //initialize serial communication at 115200 bits per second 
    while (!P1.init()) 
    {    
      ;//Wait for Modules to Sign on
    } 

    /*
       When the switch is in the ON position the watchdog will be set to TOGGLE MODE. 
       A system reset will occur after 5000 milliseconds. 

       It is required to call the configWD and startWD functions before using the watchdog timer.
     */

    if(digitalRead(SWITCH_BUILTIN) == HIGH){ //When the switch is in the ON position the following code will execute.
      P1.configWD(5000, TOGGLE); //Pass in the timer value and TOGGLE will reset the CPU after 5000 milliseconds has passed.
      Serial.println("Config 1"); //Prints config number to the serial monitor
    }

    /*
       When the switch is in the OFF position the watchdog will be set to HOLD MODE. 
       The system will stop all code from executing and the base LED will flash to 
       indicate that there is an error in the system. 
       
       All module outputs will be turned off as well for safety reasons. 
       
       To start code execution again a power cycle is required. 
     */

    else{ //When the switch is in the OFF position the following code will execute. 
      P1.configWD(5000, HOLD);    //Pass in the timer value and HOLD will stop the CPU from executing code until a powercycle.
      Serial.println("Config 2"); //Prints config number to the serial monitor
    }

  P1.startWD();             //start Watch Dog function. Timer starts at 0 and waits the previously passed in value (5000 ms). 
}


/*
   The loop below starts by setting the user LED on to signify that the processor is executing code.
   The switch position was used to determine the behavoir of the watchdog.
   
   The loop pets the watchdog to restart the timer. Afterwards a delay of 10 seconds will ensure 
   that the base restarts because the watchdog was configured to wait 5 seconds.
   
   The stopWD function call has ben commented out. This function can be used to stop the watchdog timer
   and allow the code to continue. 
 */

void loop() { // the loop routine runs over and over again forever:

  digitalWrite(LED_BUILTIN, HIGH);    //Turns the user LED on.
  P1.petWD();       //when called petWD will reset Watchdog timer to 0.
  //P1.stopWD();    //this line can be uncommented to allow the code to continue to execute.
  delay(10000);     //delay 10 seconds

}
