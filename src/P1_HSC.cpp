/*
MIT License

Copyright (c) 2023 FACTS Engineering, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "P1_HSC.h"
/*******************************************************************************	
Description: Constructor for P1_HSC_Module class. Contains functions and properties
			 that relate to the module as a whole. Each P1-02HSC module will require
		     It's own object. Sets default channel settings for P1_HSC_Channel 
			 members CNT1 and CNT2. 
			 
Parameters: -uint8_t slotSelect - Slot the module is in. Slots start at 1.
			 
Returns: 	-none

Example Code: 
*******************************************************************************/
P1_HSC_Module::P1_HSC_Module(uint8_t slotSelect){

	CNT1.channelNumber = 1;
	CNT1.slotNumber = slotSelect;
	
	CNT2.channelNumber = 2;
	CNT2.slotNumber = slotSelect;
	
	slotNumber = slotSelect;
}

/*******************************************************************************	
Description: Initialise the P1-02HSC module. Writes current settings for both
			 channels to the module and sets position to 0.
			 
Parameters: -none
			 
Returns: 	-none

Example Code: 
*******************************************************************************/
void P1_HSC_Module::configureChannels(void){
	char cfgBuf[12];
	
	memset(cfgBuf,0,12);
		
	cfgBuf[7]  = CNT1.polarity; 		//Bit 0
	cfgBuf[7] += CNT1.mode << 1;		//Bit 1-2
	cfgBuf[7] += CNT1.inhibitOn << 3; 	//Bit 3-5
	cfgBuf[7] += CNT1.enableZReset << 6;//Bit 6
	cfgBuf[7] += CNT1.isRotary << 7;	//Bit 7
		
	cfgBuf[11]  = CNT2.polarity; 			//Bit 0
	cfgBuf[11] += CNT2.mode << 1;			//Bit 1-2
	cfgBuf[11] += CNT2.inhibitOn << 3; 		//Bit 3-5
	cfgBuf[11] += CNT2.enableZReset << 6;	//Bit 6
	cfgBuf[11] += CNT2.isRotary << 7;		//Bit 7

	CNT1.setRolloverPosition(CNT1.rolloverPosition);
	CNT2.setRolloverPosition(CNT2.rolloverPosition);
	CNT1.setPosition(0);
	CNT2.setPosition(0);
	
	P1.configureModule(cfgBuf,slotNumber);
	delay(100);
}

/*******************************************************************************	
Description: Initialise the P1-02HSC module. Writes current settings for both
			 channels to the module and sets position to 0. This function
			 provides an option to use custom channel classes to init.
			 
Parameters: -P1_HSC_Channel CH1 - Channel class with settings for init
            -P1_HSC_Channel CH2 - Channel class with settings for init
			 
Returns: 	-none

Example Code: 
*******************************************************************************/
void P1_HSC_Module::configureChannels(P1_HSC_Channel CH1, P1_HSC_Channel CH2){
	char cfgBuf[12];
	
	memset(cfgBuf,0,12);
		
	cfgBuf[7]  = CH1.polarity; 		//Bit 0
	cfgBuf[7] += CH1.mode << 1;		//Bit 1-2
	cfgBuf[7] += CH1.inhibitOn << 3; 	//Bit 3-5
	cfgBuf[7] += CH1.enableZReset << 6;//Bit 6
	cfgBuf[7] += CH1.isRotary << 7;	//Bit 7
		
	cfgBuf[11]  = CH2.polarity; 			//Bit 0
	cfgBuf[11] += CH2.mode << 1;			//Bit 1-2
	cfgBuf[11] += CH2.inhibitOn << 3; 		//Bit 3-5
	cfgBuf[11] += CH2.enableZReset << 6;	//Bit 6
	cfgBuf[11] += CH2.isRotary << 7;		//Bit 7

	CH1.setRolloverPosition(CH1.rolloverPosition);
	CH2.setRolloverPosition(CH2.rolloverPosition);
	CH1.setPosition(0);
	CH2.setPosition(0);

	P1.configureModule(cfgBuf,slotNumber);
	delay(100);
}

/*******************************************************************************	
Description: Reads the 8 inputs of the P1-02HSC and returns the value as a 
			 bitmapped value. Additionally updates the bool parameters for	
			 each input that reside within the P1_HSC_Module object.
			 
Parameters: -none
			 
Returns: 	- uint8_t reading - value in the range of 8 bits representing the
			  state of each input

Example Code: 
*******************************************************************************/
uint8_t P1_HSC_Module::readInputs(void){
	uint8_t reading = 0;
	
	reading = P1.readDiscrete(slotNumber);
	inputStatus = reading & 0xFF;
	
	status1A  = (reading & 0x01);
	status1B  = (reading & 0x02) >> 1;
	status1Z  = (reading & 0x04) >> 2;
	status3IN = (reading & 0x08) >> 3;
	status2A  = (reading & 0x10) >> 4;
	status2B  = (reading & 0x20) >> 5;
	status2Z  = (reading & 0x40) >> 6;
	status4IN = (reading & 0x80) >> 7;
	
	return reading;
}

/*******************************************************************************	
Description: Constructor for P1_HSC_Channel class. Contains functions and properties
			 for each individual channel of the P1-02HSC. The P1_HSC_Module object
			 has 2 default instances of this class: CNT1 and CNT2. Additional
			 class objects may be defined to allow for more descriptive naming.
			 
Parameters: -uint8_t slotSelect - Slot the module is in. Slots start at 1.
			-uint8_t channelSelect - Channel on P1-02HSC. Should be 1 or 2.
			 
Returns: 	-none

Example Code: 
*******************************************************************************/
P1_HSC_Channel::P1_HSC_Channel(uint8_t slotSelect, uint8_t channelSelect){

	slotNumber = slotSelect;
	channelNumber = channelSelect;

}

/*******************************************************************************	
Description: Reads the current positon of the channel in counts. Value is a 
			 32 bit unsigned interger and will roll under/over at 0xFFFFFFFF
			 
Parameters: -none
			 
Returns: 	-int current position in counts

Example Code: 
*******************************************************************************/
int P1_HSC_Channel::readPosition(void){
	
	return (int)P1.readAnalog(slotNumber,channelNumber);
	
}

/*******************************************************************************	
Description: Sets the current position of a channel to a specified value
			 
Parameters: -int counts -  Position in counts
			 
Returns: 	-none

Example Code: 
*******************************************************************************/
void P1_HSC_Channel::setPosition(int counts){
	uint8_t  registerOffset = 0;
	uint32_t bitPosition = 0;
	
	if(channelNumber == 1){
		registerOffset = 5;
		bitPosition = 1;
	}
	else{
		registerOffset = 9;
		bitPosition = 0x10000;
	}
	
	P1.writeAnalog(counts,slotNumber,registerOffset);	//Set position
	P1.writeAnalog(bitPosition,slotNumber,1);//Trigger set position load bit
	P1.writeAnalog(0,slotNumber,1);	//Reset flag bit
}

/*******************************************************************************	
Description: Sets the value to reset the position to when Z-input reset(resetWithZ)
			 is enabled for the channel. This function does not reset the value,
			 it still must be triggered by the Z input
			 
Parameters: -int counts - Position in counts
			 
Returns: 	-none

Example Code: 
*******************************************************************************/
void P1_HSC_Channel::setZResetValue(int counts){	
	uint8_t registerOffset = 0;
	
	if(channelNumber == 1){
		registerOffset = 2;
	}
	else{
		registerOffset = 6;
	}
	
	P1.writeAnalog(counts,slotNumber,registerOffset);	//Set Z reset position
	
}

/*******************************************************************************	
Description: Set rotary rollover position for channel. This setting will
			 only work when using the rotary encoder setting.				
				
Parameters: -int counts - Position in counts
			 
Returns: 	-none

Example Code: 
*******************************************************************************/
void P1_HSC_Channel::setRolloverPosition(int counts){
	uint8_t registerOffset = 0;
	
	if(channelNumber == 1){
		registerOffset = 4;
	}
	else{
		registerOffset = 8;
	}
	
	
	rolloverPosition = counts;	//Store current rollover in case init is called again
	P1.writeAnalog(counts,slotNumber,registerOffset);	
	
}

/*******************************************************************************	
Description: Function to use when polling for a rollover event. This flag will
			 only trigger when using the rotary encoder setting. The rollover
			 will not be counted by this function and it must be polled continuously
			 polled to see each individual event
							
Parameters: -none
			 
Returns: 	-int rollover event. Returns 1 for roll-over, -1 for roll-under, 0 for
			 no event
			 

Example Code: 
*******************************************************************************/
int P1_HSC_Channel::readRollOver(void){
	uint8_t registerOffset = 0;
	int statusValue = 0;
	
	if(channelNumber == 1){
		registerOffset = 4;
	}
	else{
		registerOffset = 5;
	}
	
	statusValue = P1.readAnalog(slotNumber,registerOffset);	
	statusValue = (statusValue & 0x3000000) >> 24;

	if(statusValue == 0b10){
		while(statusValue == 0b10){
			statusValue = P1.readAnalog(slotNumber,registerOffset);	
			statusValue = (statusValue & 0x3000000) >> 24;
		}
		return -1; 	//Rolled under
	}
	else if(statusValue == 0b01){
		
		while(statusValue == 0b01){
			statusValue = P1.readAnalog(slotNumber,registerOffset);	
			statusValue = (statusValue & 0x3000000) >> 24;
		}
		return 1;	//Rolled Over
	}
	else{
		return 0;	//No Rolls :(
	}
}

/*******************************************************************************	
Description: Reads the 4 inputs of the P1-02HSC channel selected and returns 
			 the value as a bitmapped value. Each counter has an A,B,Z, and IN 
			 input.
			 
Parameters: -none
			 
Returns: 	- uint8_t reading - value in the range of 4 bits representing the
			  state of each input

Example Code: 
*******************************************************************************/
uint8_t P1_HSC_Channel::readInputs(void){
	uint8_t reading = 0;
	
	reading = P1.readDiscrete(slotNumber) & 0xFF;
	
	if(channelNumber == 1){
		reading &= 0xF;		//Mask lower 4 bits for channel 1
	}
	else{
		reading = reading>>4;	//Keep upper 4 bits for channel 2
	}
	
	
	return reading;
}

/*******************************************************************************	
Description: Returns the alert status bit for the channel specified.
							
Parameters: -none
			 
Returns: 	-uint32_t value of the status register. See online documentation for
			 more information.
			 

Example Code: 
*******************************************************************************/
uint32_t P1_HSC_Channel::readAlerts(void){
	uint8_t registerOffset = 0;
	
	if(channelNumber == 1){
		registerOffset = 4;
	}
	else{
		registerOffset = 5;
	}
	
	return P1.readAnalog(slotNumber,registerOffset);
	
}