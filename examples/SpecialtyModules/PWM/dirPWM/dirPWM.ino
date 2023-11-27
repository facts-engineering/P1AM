/*Example: dirPWM

  This example shows how to use the P1-04PWM module as a discrete output.
  This example will blink the output of channel 2 on the P1-04PWM module.
 
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

char configData[4] = {0x02, 0x06, 0x02, 0x02};  //passing 0x06 into the config will change that channel to be a discrete output

void setup() {  // the setup routine runs once:
  Serial.begin(115200);   //initialize serial communication at 115200 bits per second 
  while(!P1.init())
  {
    ; //wait for Modules to sign on
  }

  P1.configureModule(configData, 1);  //writes configuration data to the module in slot 1
  

}

void loop() { // the loop routine runs over and over again forever:
  P1.writePWMDir(HIGH, 1, 2); //data, slot, channel
  delay(1000);  //waits 1 second
  P1.writePWMDir(LOW, 1, 2); //data, slot, channel
  delay(1000);  //waits 1 second
}
