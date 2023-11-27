/*
  Example: pwmOutput

  This example shows how to use the P1-04PWM module.
  This example sets duty cycle, frequency on the P1-04PWM.

  The P1-04PWM module will output a pulse width modulated signal on channel 2.
  The duty cycle is set to 50.15 and the frequency is set to 10kHz.
  The output being active is shown by the LED being on.
  The output of the module can be verified by using an oscilloscope.
 
  This example works with the P1-04PWM in slot 1.
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
  Serial.begin(115200);   //initialize serial communication at 115200 bits per second 
  while(!P1.init())
  {
    ; //wait for Modules to sign on
  }

}

float dutyCycle = 50.15;  //sets dutyCycle variable to 50.15
int frequency = 10000;  //sets frequency variable to 10000Hz

void loop() { // the loop routine runs over and over again forever:
  
  /*
   writePWM can set both the duty cycle and frequency at the same time
    
   accepts floating point values for duty cycle (upto 2 decimal places)
   frequency is an integer value between (?-?)
   */
  P1.writePWM(dutyCycle, frequency, 1, 2);  //The values to be passed in are duty cycle, frequency, slot, channel
  //P1.writePWMDuty(dutyCycle, 1, 2); //writePWMDuty can be used to change only the dutycycle
  //P1.writePWMFreq(frequency, 1, 2); //writePWMFreq can be used to change only the frequency

  delay(5000);  //waits 5 seconds
}
