/*
 Example: ModuleConfiguration
 
 This example shows how to configure modules.
 Configuration is written to a module to change things such as:
 - number of active channels
 - type of input e.g. thermocouple type
 - input or output range
 
 This example works with all P1000 Series:
  - Analog input modules that can be configured such as P1-04ADL-1, P1-04ADL-2, etc.
  - Other modules that can be configured
  
  This example will set the module configuration to only have channel 1 active. 
  Afterwards it will print the configuration status to the serial monitor and then the
  channel data from channel 1.
 
 Configuration data for P1000 modules can be found on https://facts-engineering.github.io/config.html 
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

const char configData[2] = {0x40, 0x00};      //Creates an array to store module configuration data. 0x4000 will enable only channel 1 as an input. 

void setup(){  // the setup routine runs once
  
  Serial.begin(115200);   //initialize serial communication at 115200 bits per second 
  while(!P1.init()){
    ; //wait for Modules to sign on
  }
  
  P1.configureModule(configData, 1);     //sends the config data to the module in slot 1
}

int channelData;    //variable to store analog data read from module
char readConfigData[2]; //array to store configuration data read from module

void loop(){ // the loop routine runs over and over again forever:

  P1.readModuleConfig(readConfigData, 1);  //reads the configuration data of the module in slot 1

  Serial.print("Config Byte 0: 0x");
  Serial.println(readConfigData[0],HEX);   //prints the read configuration data to confirm it was written
  Serial.print("Config Byte 1: 0x");
  Serial.println(readConfigData[1],HEX);   //prints the read configuration data to confirm it was written

  channelData = P1.readAnalog(1, 1);  //read channel 1 of slot 1

  Serial.print("Channel 1 = ");
  Serial.println(channelData);        //print channel data

  delay(2000);
}
