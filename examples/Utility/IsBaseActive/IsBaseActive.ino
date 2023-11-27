/*
  Example: IsBaseActive
  
  This example will use the "P1.isBaseActive" functions to demonstrate how the Base Controller 
  can be monitored for a loss of 24V power. When this occurs, the Base Controller will need to be 
  re-initialized  using P1.init(). USB will need to be connected for serial communication and power.
  
  This example works with all P1000 Series:
    - Discrete output modules such as P1-08TRS, P1-08TD1, P1-16TR, etc.
    - Discrete combo modules such as P1-15CDD1, P1-15CDD2, P1-16CDR, etc.  
  
  This example will toggle channel 2 of slot 1 on and off every second after a base reset. 

 //To detect a base failure and re-init
 Step 1: Load program in current form. 
 Step 2: Connect USB cable from PC to P1AM-CPU. 
 Step 3: Remove 24Vdc power and reapply. *Note: Open serial Monitor. You should have no errors. 
 Step 4: Verify Slot 1 Channel 2 output is blinking.
 
 // To see a failure
 Step 5: Comment out lines 55-62.
 Step 6: Upload to the P1AM-CPU.
 Step 7: Remove 24Vdc power & reapply.*Note: Open serial Monitor you will get a "Base Sync Timeout" error. 
 Step 8: The output on Slot 1 Channel 2 should not be blinking.

  It will also provide error feedback to the serial monitor on loss of 24Vdc.

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
                 
  Written by FACTS Engineering and AutomationDirect. 
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

void loop(){ // the loop routine runs over and over again forever:
  if(P1.isBaseActive() == false){
    Serial.println("Re-init() the base modules.");
    delay(10);
    while (!P1.init()){ 
      Serial.println("Waiting for 24V");
      delay(1000);
    }
  }
  P1.writeDiscrete(HIGH,1,2);  //Turn slot 1 channel 2 on
  delay(1000);  //wait 1 second
  P1.writeDiscrete(LOW,1,2);   //Turn slot 1 channel 2 off
  delay(1000);  //wait 1 second
  
}