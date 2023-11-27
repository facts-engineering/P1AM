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

#include "P1AM.h"

SPISettings P100_SPI_SETTINGS(1000000, MSBFIRST, SPI_MODE2);

P1AM::P1AM(){
	pinMode(slaveSelectPin, OUTPUT);		//Define Slave select pin for Base Controller
	pinMode(slaveAckPin, INPUT);			//Define Ack pin for Base Controller
	pinMode(baseEnable, OUTPUT);			//Define baseEnable pin used by Arduino to enable Base Controller
}

P1AM P1;		//Create Class instance

/*******************************************************************************
Description: Initialise the P1AM-100 Base Controller. It automatically configures each module
			 with default settings and returns the number of modules that have signed on.

Parameters: -None

Returns: 	-uint8_t - Number of good modules that signed on.
*******************************************************************************/
uint8_t P1AM::init() {
	uint32_t slots = 0;
	int dbLoc = 0;
	char *cfgArray;
	const uint8_t max_p1_slots = 15;
	
	union moduleIDs{		// Use a union as a quick convert between byte arrays and ints
		uint32_t IDs[max_p1_slots];
		uint8_t byteArray[max_p1_slots * 4];
	}modules;
	uint8_t retry = 0;


	memset(baseSlot,0,sizeof(baseSlot));	//Clear base constants array
	enableBaseController(HIGH);	//Start base controller
	delay(100);

	if(spiTimeout(1000*5000) == false){
		Serial.println("No Base Controller Activity");
		Serial.println("Check External Supply Connection");
		delay(1000);
		return 0;
	}

	while(((slots == 0) || (slots > 15)) && (retry < 5)){	//Begin sign-on until number of slots are valid. Only retry 5 times.
		if(handleHDR(MOD_HDR)){
			delay(5);
			slots = spiSendRecvByte(DUMMY);		//Get number of modules in base
			if(slots == 0 || slots > 15){
				if(retry > 2){
					enableBaseController(LOW);	//Disable base controller
					delay(10);
					enableBaseController(HIGH);	//Start base controller
					delay(10);
				}
				retry++;		//Let Base Controller retry
			}
		}
	}

	if(retry >= 5){		//Zero module in the base. Quit sign-on routine and let the user know
		Serial.println("Zero modules in the base");
		delay(500);
		return 0;
	}
	
	if(slots >  NUMBER_OF_MODULES){
		Serial.println("\nToo many modules in Base");
		Serial.print("Device only supports ");
		Serial.println(NUMBER_OF_MODULES);
		memset(baseSlot, 0, sizeof(baseSlot));	//Clear base constants array
		return 0;
	}

	spiTimeout(1000*200);
	spiSendRecvBuf(modules.byteArray,slots*4,1);		//slots * 4 bytes per ID code

	uint8_t baseControllerConstants[1 * max_p1_slots * 7];		//seven elements in module sign on
	for(uint32_t i=0;i<slots;i++){
		dbLoc = 0;
		while (modules.IDs[i] != mdb[dbLoc].moduleID){		//Scan MDB for matching ID and grab its array location
			if(mdb[dbLoc].moduleID == 0xFFFFFFFF){
				debugPrintln("Module is not in Module List");	//If we got here, we probably need to update the library.
				break;
			}
			dbLoc++;
		}
		baseSlot[i].dbLoc = dbLoc;		//MDB Location

		//Grab MDB values and load them into variables for P1AM-100 and array to send to Base Controller
		baseControllerConstants[0+i*7]  = mdb[dbLoc].diBytes;
		baseControllerConstants[1+i*7]  = mdb[dbLoc].doBytes;
		baseControllerConstants[2+i*7]  = mdb[dbLoc].aiBytes;
		baseControllerConstants[3+i*7]  = mdb[dbLoc].aoBytes;
		baseControllerConstants[4+i*7]  = mdb[dbLoc].statusBytes;
		baseControllerConstants[5+i*7]  = mdb[dbLoc].configBytes;
		baseControllerConstants[6+i*7]  = mdb[dbLoc].dataSize;
	}

	spiTimeout(1000*200);
	delay(1);
	spiSendRecvBuf(baseControllerConstants,slots*7);	//Send mdb values to Base Controller
	delay(10);

	#ifndef AUTO_CONFIG_OFF
	for (uint32_t i = 0; i < slots; i++){ 	//default config routine
		dbLoc = baseSlot[i].dbLoc;
		if(mdb[dbLoc].configBytes > 0){				//Modules with config Bytes need to havea config loaded
			cfgArray = loadConfigBuf(mdb[dbLoc].moduleID);	//Get pointer to default config for this module
			while(!configureModule(cfgArray, i + 1)){		//configure module
				debugPrintln("Working");
			}
		}
	}
	#endif

	delay(50);				//Let the Base Controller complete its end of the sign-on
	#ifdef DEBUG_PRINT_ON
		slots = printModules();	//Returns the number of good modules. Prints IDs to serial monitor
	#endif
	return slots;
}

/*******************************************************************************
Description: Enables or disables base controller during normal operation.

Parameters: -bool state - On or off control

Returns: 	-none
*******************************************************************************/
void P1AM::enableBaseController(bool state){

	digitalWrite(baseEnable, state);

}

/*******************************************************************************
Description: Checks to see if modules in base appear as expected. Disables any modules
			 that are found not to comply. This function only works after init.

Parameters: -char* moduleNames[] - List of Module names in order they appear in base

Returns: 	-uint16_t - Bitmapped representation of errors. A one in any position
					    represents a error in configuration. E.g. an error in
					    slot 1 and 3 would return 0x05.
*******************************************************************************/
uint16_t P1AM::rollCall(const char* moduleNames[], uint8_t numberOfModules){
	uint16_t slotError = 0;
	uint8_t dbLoc = 0;
	bool singleError;


	for(int i = 0;i < numberOfModules;i++){
		dbLoc = baseSlot[i].dbLoc;

		singleError = (moduleNames[i] != mdb[dbLoc].moduleName);//Set to true if there is an error
		slotError = slotError | (singleError << i);	//Shift bit of error check and add to our return
		if(singleError == true){
			debugPrint("Slot ");
			debugPrint(i+1);
			debugPrintln(" Module Mismatch");

			debugPrint("Expected: ");
			debugPrintln(moduleNames[i]);

			debugPrint("Found: ");
			debugPrintln(mdb[dbLoc].moduleName);

			debugPrintln();	//CRLF for next message

			baseSlot[i].dbLoc = 0;	//Clear out slot so it no longer functions
		}
	}

	if(slotError == 0){
		debugPrintln("All Modules Good");
	}

	return slotError;

}


/*******************************************************************************
Description: Read a single discrete input module

Parameters: -uint8_t slot - Slot to read from. Slots start at 1.
			-(Optional) uint8_t channel = 0. If specified reads from "channel".
			  If left out or 0, reads "data" from all channels where
			  least Signficant bit is channel 1.

Returns: 	-uint32_t data read from the module
*******************************************************************************/
uint32_t P1AM::readDiscrete(uint8_t slot, uint8_t channel){
	uint32_t data = 0;
	uint8_t len = 0;
	uint8_t mdbLoc = 0;
	char rData[4] = {0,0,0,0};

	mdbLoc = baseSlot[slot-1].dbLoc;
	len = mdb[mdbLoc].diBytes;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return 0;
	}

	if((len <= 0)){
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module has no Discrete Input bytes");
		return 0;
	}

	if(channel > (len * 8)){		//8 channels per byte
		debugPrintln("This channel is not valid");
		return 0;
	}

	rData[0] = READ_DISCRETE_HDR;
	rData[1] = slot;
	spiSendRecvBuf((uint8_t *)rData,2);
	memset(rData,0,4);		//clear buffer
	if(spiTimeout(1000*200) == true){
		spiSendRecvBuf((uint8_t *)rData,len,true);
		data  = (rData[3]<<24);
		data += (rData[2]<<16);
		data += (rData[1]<<8);
		data += (rData[0]<<0);

		if(channel != 0){
			data = (data>>(channel-1)) & 1;	// shift and mask
		}
		dataSync();
		return data;
	}
	else{
		debugPrintln("Slow read");
		delay(100);
		return 0;
	}
}

/*******************************************************************************
Description: Write to single discrete output module

Parameters: -uint32_t data - Data to write to module
			-uint8_t slot - Slot to write to. Slots start at 1.
			-(Optional) uint8_t channel = 0. If specified writes to "channel".
			  If left out or 0, writes "data" to all channels where
			  least Signficant bit is channel 1.

Returns: 	-None
*******************************************************************************/
void P1AM::writeDiscrete(uint32_t data,uint8_t slot, uint8_t channel){
	uint8_t tData[7];
	uint8_t mdbLoc = 0;
	uint8_t len = 0;

	mdbLoc = baseSlot[slot-1].dbLoc;
	len = mdb[mdbLoc].doBytes;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return;
	}

	if((len <= 0)){
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module has no Discrete Output bytes");
		return;
	}

	if(channel > (len * 8)){		//8 channels per byte
		debugPrintln("This channel is not valid");
		return;
	}

	tData[0] = WRITE_DISCRETE_HDR;
	tData[1] = slot;
	tData[2] = channel;

	if(channel == 0){
		for(int i=0;i<len;i++){
			tData[i+3] = data>>(8*i)  & 0xFF;	//Shift and mask
		}
	}
	else{
		tData[3] = data & 0b1;			//mask bit
		len = 1;						//only send 1 byte
	}


	spiSendRecvBuf(tData,len+3);	//3 Header bytes plus data length

	dataSync();
	return;
}

/*******************************************************************************
Description: Read a single analog input module channel.

Parameters: -uint8_t slot - Slot to read from. Slots start at 1.
			-uint8_t channel - Channel to read from. Channels start at 1.

Returns: 	-uint32_t - Value of channel in counts. 12-bit returns 0-4095,
			 16-bit returns between 0-65535, etc.
*******************************************************************************/
int P1AM::readAnalog(uint8_t slot, uint8_t channel){
	int data = 0;
	uint8_t len = 0;
	uint8_t mdbLoc = 0;
	char rData[4] = {0,0,0,0};

	mdbLoc = baseSlot[slot-1].dbLoc;
	len = mdb[mdbLoc].aiBytes;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return 0;
	}

	if((len <= 0)){
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module has no Analog Input bytes");
		return 0;
	}

	if((channel <= 0) || (channel > (len / 4))){		//4 bytes per channel
		debugPrintln("This channel is not valid");
		return 0;
	}

	rData[0] = READ_ANALOG_HDR;
	rData[1] = slot;
	rData[2] = channel;
	spiSendRecvBuf((uint8_t *)rData,3);

	if(spiTimeout(1000*200) == true){
		data = spiSendRecvInt(DUMMY);
		dataSync();
		return data;
	}
	else{
		debugPrintln("Slow read");
		delay(100);
		return 0;
	}
}

/*******************************************************************************
Description: Read a single temperature input module channel.

Parameters: -uint8_t slot - Slot to read from. Slots start at 1.
			-uint8_t channel - Channel to read from. Channels start at 1.
Returns: 	-Value of channel in degrees for temperature. mV for voltages.
*******************************************************************************/
float P1AM::readTemperature(uint8_t slot, uint8_t channel){
	union int2float{
		int data;			//We get an int back no matter what analog module. Unions set variables as the same/
		float temperature;	//Floats and Ints are both 32 bits, so this lets quickly convert our int to a float.
	}ourValue;

	ourValue.data = readAnalog(slot,channel);	//Use the same analog read function to get int

	return ourValue.temperature;		//return the float
}

/*******************************************************************************
Description: Write to a single analog output module channel.

Parameters: -uint32_t data - Value of channel in counts. 12-bit in range 0-4095,
			 16-bit in range 0-65535, etc.
			-uint8_t slot - Slot to write to. Slots start at 1.
			-uint8_t channel - Channel to write to. Channels start at 1.
Returns: 	-None
*******************************************************************************/
void P1AM::writeAnalog(uint32_t data,uint8_t slot, uint8_t channel){
	uint8_t tData[7];
	uint8_t mdbLoc = 0;
	uint8_t len = 0;

	mdbLoc = baseSlot[slot-1].dbLoc;
	len = mdb[mdbLoc].aoBytes;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return;
	}

	if((len <= 0)){
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module has no Analog Output bytes");
		return;
	}

	if((channel <= 0) || (channel > (len / 4))){		//4 bytes per channel
		debugPrintln("This channel is not valid");
		return;
	}

	tData[0] = WRITE_ANALOG_HDR;
	tData[1] = slot;
	tData[2] = channel;
	tData[3] = (data>>0)  & 0xFF;
	tData[4] = (data>>8)  & 0xFF;
	tData[5] = (data>>16) & 0xFF;
	tData[6] = (data>>24) & 0xFF;
	spiSendRecvBuf(tData,7);

	dataSync();
	return;
}

/*******************************************************************************
Description: Read a block of data stored in Base Controller. This allows you to read data
			 from many modules in one command, but requires you to calculate the
			 offset and lengths and later parse out the data. You can also read
			 the state of output data through this for debugging purposes. This
			 function is more advanced and is not recommended for new programmers.

Parameters: -char buf[] - Pointer to an array that will hold the values read. This
			 function does not return a value, but stores in the *buf passed in.
			-uint16_t len - Number of bytes to read
			-uint8_t offset - Starting byte in array to read.
			-uint8_t type - Specifies which Base Controller data block to read from.
			 0 is Discrete Input, 1 is Analog Input, 2 is Discrete Output, 3 is Analog Output,4 is Status.

Returns: 	-None
*******************************************************************************/
void P1AM::readBlockData(char buf[], uint16_t len,uint16_t offset, uint8_t type){
	uint8_t readParams[6];

	if((len+offset) > 1200){		//max of data array is 1200, so we can't read past that
		len = 1200-offset;		//adjust len in case we're trying to read too far
	}

	readParams[0] = READ_BLOCK_HDR;
	readParams[1] = type;		// 0 is Discrete In, 1 is Analog In, 2 is Discrete Out, 3 is Analog Out,4 is Status
	readParams[2] = len >> 8;
	readParams[3] = len & 0xFF;
	readParams[4] = offset >> 8;
	readParams[5] = offset & 0xFF;
	spiSendRecvBuf(readParams,6);			//Send paramters for block read

	if(spiTimeout(1000*200) == true){
		spiSendRecvBuf((uint8_t *)buf,len,true);		//data is stored in buffer passed in
		dataSync();
		return;
	}
	else{
		debugPrintln("Slow block read");
		delay(100);
		return;
	}
}

/*******************************************************************************
Description: Write to a block of data stored in Base Controller. This allows you to write data
			 to many modules in one command, but requires you to calculate the
			 offset and lengths and parse together the data. You can write to input data,
			 but be aware the Base Controller may overwrite this data in normal operations.
			 This function is more advanced than others and is not recommended
			 for new programmers.

Parameters: -char buf[] - Pointer to an array that holds the values to write. This
			 array is not overwritten.
			-uint16_t len - Number of bytes to write
			-uint8_t offset - Starting byte in array to write.
			-uint8_t type - Specifies which Base Controller data block to write to.
			0 is Discrete Input, 1 is Analog Input, 2 is Discrete Output, 3 is Analog Output,4 is Status.

Returns: 	-None
*******************************************************************************/
void P1AM::writeBlockData(char buf[], uint16_t len,uint16_t offset, uint8_t type){

	if((len+offset) > 1200){		//max of data array is 1200, so we can't read past that
		len = 1200-offset;		//adjust len in case we're trying to read too far
	}
	uint8_t *tData = (uint8_t *)malloc(len+6);
	tData[0] = WRITE_BLOCK_HDR;
	tData[1] = type;		
	tData[2] = len >> 8;
	tData[3] = len & 0xFF;
	tData[4] = offset >> 8;
	tData[5] = offset & 0xFF;
	memcpy(tData+6,buf,len);
	spiSendRecvBuf(tData,len+6,0);	//data is not stored in passed buffer
	free(tData);
	dataSync();
	return;
}

/*******************************************************************************
Description: Set both duty cycle and frequency of a PWM output module channel.

Parameters: -float duty - Duty cycle. Range 0.00-100.00. You can include up to 2 decimal places
			-uint32_t freq - Frequency. See module data sheet for range. P1-04PWM is 0-20kHz.
			-uint8_t slot - Slot to write to. Slots start at 1.
			-uint8_t channel - Channel to write to. Channels start at 1.

Returns: 	-None
*******************************************************************************/
void P1AM::writePWM(float duty,uint32_t freq,uint8_t slot,uint8_t channel){
	uint8_t mdbLoc = 0;
	uint8_t tempLoc = 0;
	uint8_t offset = 0;
	uint32_t dutyInt = 0;
	char tData[8];

	mdbLoc = baseSlot[slot-1].dbLoc;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return;
	}

	if((channel <= 0) || (channel > 4)){		
		debugPrintln("This channel is not valid");
		return;
	}

	if((mdb[mdbLoc].dataSize & 0xF0) !=  0xA0){		//Is this PWM
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module is not a PWM module");
		return;		//Not PWM
	}

	for(int i=0;i<slot-1;i++){
		tempLoc = baseSlot[i].dbLoc;
		if(mdb[tempLoc].aoBytes > 0){
			offset += mdb[tempLoc].aoBytes;		//get offset of analog bytes
		}
	}

	offset += (channel - 1) * 8;	//Each channel uses 8 bytes
	dutyInt = (uint32_t)(duty * 100);// shift decimal over 2 places and cast off remainder. e.g. 12.3456 turns into 1234

	tData[3] = (dutyInt>>0)  & 0xFF;	//shift and mask to bytes
	tData[2] = (dutyInt>>8)  & 0xFF;
	tData[1] = (dutyInt>>16) & 0xFF;
	tData[0] = (dutyInt>>24) & 0xFF;
	tData[7] = (freq>>0)  & 0xFF;
	tData[6] = (freq>>8)  & 0xFF;
	tData[5] = (freq>>16) & 0xFF;
	tData[4] = (freq>>24) & 0xFF;

	writeBlockData(tData, 8, offset, ANALOG_OUT_BLOCK); //Use block data to ensure duty/freq are entered at the same time.
	dataSync();

	return;
}

/*******************************************************************************
Description: Set duty cycle of a PWM output module channel.

Parameters: -float duty - Duty cycle. Range 0.00-100.00. You can include up to 2 decimal places
			-uint8_t slot - Slot to write to. Slots start at 1.
			-uint8_t channel - Channel to write to. Channels start at 1.

Returns: 	-None
*******************************************************************************/
void P1AM::writePWMDuty(float duty,uint8_t slot,uint8_t channel){
	uint8_t mdbLoc = 0;
	uint32_t dutyInt = 0;

	mdbLoc = baseSlot[slot-1].dbLoc;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return;
	}
	
	if((channel <= 0) || (channel > 4)){		
		debugPrintln("This channel is not valid");
		return;
	}

	if((mdb[mdbLoc].dataSize & 0xF0) !=  0xA0){		//Is this PWM
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module is not a PWM module");
		return;		//Not PWM
	}

	dutyInt = (uint32_t)(duty * 100);// shift decimal over 2 places and cast off remainder. e.g. 12.3456 turns into 1234
	channel = 1 + ((channel-1) * 2);
	P1.writeAnalog(dutyInt,slot,channel);

	dataSync();
	return;
}

/*******************************************************************************
Description: Set frequency of a PWM output module channel.

Parameters: -uint32_t freq - Frequency. See module data sheet for range. P1-04PWM is 0-20kHz.
			-uint8_t slot - Slot to write to. Slots start at 1.
			-uint8_t channel - Channel to write to. Channels start at 1.

Returns: 	-None
*******************************************************************************/
void P1AM::writePWMFreq(uint32_t freq,uint8_t slot,uint8_t channel){
	uint8_t mdbLoc = 0;

	mdbLoc = baseSlot[slot-1].dbLoc;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return;
	}
	
	if((channel <= 0) || (channel > 4)){		
		debugPrintln("This channel is not valid");
		return;
	}

	if((mdb[mdbLoc].dataSize & 0xF0) !=  0xA0){		//Is this PWM
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln("This module is not a PWM module");
		return;		//Not PWM
	}

	channel = 2 + ((channel-1) * 2);
	P1.writeAnalog(freq,slot,channel);

	dataSync();
	return;
}

 /*******************************************************************************
Description: Set direction of a PWM module channel when set to DIR mode.
			 See configuration document to see how to enable this feature.

Parameters: -bool data - Set channel on or off
			-uint8_t slot - Slot to write to. Slots start at 1.
			-uint8_t channel - Channel to write to. Channels start at 1.

Returns: 	-None
*******************************************************************************/
void P1AM::writePWMDir(bool data,uint8_t slot, uint8_t channel){
	 uint8_t mdbLoc = 0;

	mdbLoc = baseSlot[slot-1].dbLoc;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return;
	}
	
	if((channel <= 0) || (channel > 4)){		
		debugPrintln("This channel is not valid");
		return;
	}

	if((mdb[mdbLoc].dataSize & 0xF0) !=  0xA0){		//Is this PWM
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module is not a PWM module");
		return;		//Not PWM
	}

	channel = 1 + ((channel-1) * 2);
	P1.writeAnalog(data,slot,channel);

	dataSync();
	return;
}


/*******************************************************************************
Description: Print the names of all the modules in the base to the serial monitor

Parameters: -None

Returns: 	-Number of good sign-ons
*******************************************************************************/
uint8_t P1AM::printModules(){
	uint32_t slot = 0;
	uint8_t dbLoc = 0;
	uint8_t goodSlots = 0;

	while(baseSlot[slot].dbLoc != 0 && slot < NUMBER_OF_MODULES){
		dbLoc = baseSlot[slot].dbLoc;
		if(mdb[dbLoc].moduleID == 0 || mdb[dbLoc].moduleID == 0xFFFFFFFF){
			Serial.print("Slot ");
			Serial.print(slot + 1);
			Serial.println(":  Sign On Error");
		}
		else{
			Serial.print("Slot ");
			Serial.print(slot + 1);
			Serial.print(":  ");
			Serial.println(mdb[dbLoc].moduleName);
			goodSlots++;
		}
		slot++;
	}
	dataSync();
	Serial.println("");	//Print an empty line to give us some breathing room later
	return goodSlots;
}

/*******************************************************************************
Description: Return the module properties for a given slot

Parameters: -uint8_t slot - Slot of which you want the module parameters.
			    Slot 1 is module closest to processor.

Returns:    -moduleProps - moduleProps structure containing the specific paramters
                           at the give slot.
*******************************************************************************/
moduleProps P1AM::readSlotProps(uint8_t slot){
	
	uint8_t dbLoc = 0;
	moduleProps _slotProps;
	
	if((slot < 1) || (slot > 15)){
		debugPrintln("Slots must be between 1 and 15");
		_slotProps = mdb[0];
		return _slotProps;
	}

	if (baseSlot[slot-1].dbLoc != 0) {
		dbLoc = baseSlot[slot-1].dbLoc;
		_slotProps = mdb[dbLoc];
	}
	else {
		_slotProps = mdb[0];
	}
	return _slotProps;
}

/*******************************************************************************
Description: Read a single status byte from a single module

Parameters: -int byteNum - Which status byte to read in. Byte numbering starts at 0.
			-int slot - Which slot to read from

Returns: 	-char - The status byte.
*******************************************************************************/
char P1AM::readStatus(int byteNum,int slot){
	uint8_t len = 0;
	uint8_t mdbLoc = 0;
	uint8_t buf[1];
	uint8_t rData[4];

	mdbLoc = baseSlot[slot-1].dbLoc;
	len = 1;	//only need 1 byte

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return 0;
	}

	if(mdb[mdbLoc].statusBytes <= 0){
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module has no Status bytes");
		return 0;		//No status Bytes
	}

	rData[0] = READ_STATUS_HDR;
	rData[1] = slot;
	rData[2] = len;
	rData[3] = byteNum;//offset
	spiSendRecvBuf((uint8_t *)rData,4);

	if(spiTimeout(1000*200) == true){
		spiSendRecvBuf((uint8_t *)buf,len,true);	//data is stored in buffer passed in
		dataSync();
		return buf[0];
	}
	else{
		debugPrintln("Slow status read");
		delay(100);
		return 0;
	}
}

/*******************************************************************************
Description: Read all status bytes from a single module

Parameters: -char buf[] - Array to store the read status bytes in. This function
			does not return a value, but places it in the array passed in
			-uint8_t slot - Slot that you want to read status bytes from

Returns: 	-None
*******************************************************************************/
void P1AM::readStatus(char buf[], uint8_t slot){
	uint8_t len = 0;
	uint8_t mdbLoc = 0;
	uint8_t rData[4];

	mdbLoc = baseSlot[slot-1].dbLoc;
	len = mdb[mdbLoc].statusBytes;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return;
	}

	if(len <= 0){
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module has no Status bytes");
		return;		//No status Bytes
	}

	rData[0] = READ_STATUS_HDR;
	rData[1] = slot;
	rData[2] = len;
	rData[3] = 0;//offset
	spiSendRecvBuf((uint8_t *)rData,4);

	if(spiTimeout(1000*200) == true){
		spiSendRecvBuf((uint8_t *)buf,len,true);	//data is stored in buffer passed in
		dataSync();
		return;
	}
	else{
		debugPrintln("Slow status read");
		delay(100);
		return;
	}
}

/*******************************************************************************
Description: Checks the under range error on select P1000 modules. Returns if channel
			 is under range. This function does not check if the module supports 
			 this feature.

Parameters: -uint8_t slot - Which slot to read from.
			-(Optional) uint8_t channel = 0. If value is non-zero checks the 
			  specified channel. If left out or 0, checks all channels where
			  least Signficant bit is the status of channel 1.
			

Returns: 	-uint8_t - returns under range status
*******************************************************************************/
uint8_t P1AM::checkUnderRange(uint8_t slot, uint8_t channel){
	uint8_t statusByte = 0;
	
	statusByte = readStatus(UNDER_RANGE_STATUS,slot);
	if(channel){
		statusByte = (statusByte >> (channel - 1)) & 1;	//shift and mask bits
	}
	
	return statusByte;
}

/*******************************************************************************
Description: Checks the over range error on select P1000 modules. Returns if channel
			 is over range. This function does not check if the module supports 
			 this feature.

Parameters: -uint8_t slot - Which slot to read from.
			-(Optional) uint8_t channel = 0. If value is non-zero checks the 
			  specified channel. If left out or 0, checks all channels where
			  least Signficant bit is the status of channel 1.
			

Returns: 	-uint8_t - returns over range status
*******************************************************************************/
uint8_t P1AM::checkOverRange(uint8_t slot, uint8_t channel){
	uint8_t statusByte = 0;
	
	statusByte = readStatus(OVER_RANGE_STATUS,slot);
	if(channel){
		statusByte = (statusByte >> (channel - 1)) & 1;	//shift and mask bits. 
	}
	
	return statusByte;
}

/*******************************************************************************
Description: Checks the burnout error on select P1000 modules. Returns if channel
			 is over range. This function does not check if the module supports 
			 this feature.

Parameters: -uint8_t slot - Which slot to read from.
			-(Optional) uint8_t channel = 0. If value is non-zero checks the 
			  specified channel. If left out or 0, checks all channels where
			  least Signficant bit is the status of channel 1.
			

Returns: 	-uint8_t - returns burnout status
*******************************************************************************/
uint8_t P1AM::checkBurnout(uint8_t slot, uint8_t channel){
	uint8_t statusByte = 0;
	
	statusByte = readStatus(BURNOUT_STATUS,slot);
	if(channel){
		statusByte = (statusByte >> (channel - 1)) & 1;	//shift and mask bits
	}
	
	return statusByte;
}

/*******************************************************************************
Description: Checks the lost 24V error on  select P1000 modules. Returns 1 if slot is missing
			 24V. This function does not check if the module supports this feature.

Parameters: -uint8_t slot - Which slot to read from.

Returns: 	-uint8_t - 1 if 24V is missing. 0 if 24V is present
*******************************************************************************/
uint8_t P1AM::check24V(uint8_t slot){
	uint8_t statusByte = 0;
	
	statusByte = readStatus(MISSING24V_STATUS,slot);
	statusByte = (statusByte >> 1) & 1;	//Mask and shift for 24V bit
	
	return statusByte;
}

/*******************************************************************************
Description: Manually configure a module. Use this if you want to use a setting
			 that is not the default for a module. 
			 Visit https://facts-engineering.github.io/config.html for more information

Parameters: -uint8_t slot - Slot you want to configure.
			-char *cfgData - Pointer to array that contains the configuration settings

Returns: 	-bool - Configuration successfully written to Base Controller.
*******************************************************************************/
bool P1AM::configureModule(char cfgData[], uint8_t slot){
	uint8_t len = 0;
	uint8_t mdbLoc = 0;
	uint8_t cfgForSpi[66];

	mdbLoc = baseSlot[slot-1].dbLoc;
	len = mdb[mdbLoc].configBytes + 2;
	cfgForSpi[0] = CFG_HDR;
	cfgForSpi[1] = slot;

	memcpy(cfgForSpi+2,cfgData,len);
	delay(1);
	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return 0;
	}

	if(len <= 0){
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module has no config Bytes");
		return 0;
	}

	spiSendRecvBuf(cfgForSpi,len);
	delay(100);		//Additional time for Config to written
	dataSync();
	dataSync();
	return 1;
}

/*******************************************************************************
Overload of above function for const arrays
*******************************************************************************/
bool P1AM::configureModule(const char cfgData[], uint8_t slot){
	
	return configureModule((char*)cfgData, slot);
	
}

/*******************************************************************************
Description: Read current configuration of a module. Data is stored in the passed
			 in array pointer.


Parameters: -char *cfgData - Pointer to array that will receive the current configuration
			-uint8_t slot - Slot you want to configure.

Returns: 	-none
*******************************************************************************/
void P1AM::readModuleConfig(char cfgData[], uint8_t slot){
	uint8_t len = 0;
	uint8_t mdbLoc = 0;
	uint8_t cfgForSpi[2];

	mdbLoc = baseSlot[slot-1].dbLoc;
	len = mdb[mdbLoc].configBytes;

	if((slot < 1) || (slot > NUMBER_OF_MODULES)){
		debugPrint("Slots must be between 1 and ");
		debugPrintln(NUMBER_OF_MODULES);
		return;
	}

	if(len <= 0){
		debugPrint("Slot ");
		debugPrint(slot);
		debugPrintln(": This module has no config Bytes");
		return;
	}

	cfgForSpi[0] = READ_CFG_HDR;
	cfgForSpi[1] = slot;
	spiSendRecvBuf((uint8_t *)cfgForSpi,2);

	if(spiTimeout(1000*200) == true){
		spiSendRecvBuf((uint8_t *)cfgData,len,true);	//data is stored in buffer passed in
		dataSync();
		return;
	}
	else{
		debugPrintln("Slow config read");
		delay(100);
		return;
	}
}

/*******************************************************************************
Description: Configures the behaviour of the watchdog timer.

Parameters: -uint16_t milliseconds - Time in milliseconds in range 0-65535.
			 If any P1AM-100 function is not called before the time elapses,
			 the Base Controller will toggle the reset pin on the P1AM-100.

Returns: 	-None
*******************************************************************************/
void P1AM::configWD(uint16_t milliseconds, uint8_t toggle) {
	uint8_t lowByte;
	uint8_t highByte;
	uint8_t lowToggle;
	uint8_t highToggle;
	uint16_t toggleTime = 100;
	uint8_t wdData[6];

	lowByte = (uint8_t) milliseconds;
	highByte = (uint8_t) (milliseconds >> 8);

	lowToggle = (uint8_t) toggleTime;
	highToggle = (uint8_t) (toggleTime >> 8);

	wdData[0] = CONFIGWD_HDR;
	wdData[1] = lowByte;
	wdData[2] = highByte;
	wdData[3] = lowToggle;
	wdData[4] = highToggle;
	wdData[5] = toggle & 0x01;

	spiSendRecvBuf(wdData,6);
	dataSync();
	return;
}

/*******************************************************************************
Description: Begin the watchdog timer. If the watchdog is not configured beforehand,
			 Base Controller WD will default to fullscale (65535) timer. If the Base Controller is not
			 written to within the time period, the Base Controller will reset the P1AM-100.

Parameters: -None

Returns: 	-None
*******************************************************************************/
void P1AM::startWD() {

	spiSendRecvByte(STARTWD_HDR);		//Send "StartWD" header to Base Controller
	if(spiTimeout(1000*200) == true){
		spiSendRecvByte(DUMMY);		//Send "PetWD" header to Base Controller
		dataSync();
	}
	return;
}

/*******************************************************************************
Description: Stops the watchdog timer from running. Use this is you have code
			 that takes a long time to execute and you are unable to add petWD
			 calls to.

Parameters: -None

Returns: 	-None
*******************************************************************************/
void P1AM::stopWD() {

	spiSendRecvByte(STOPWD_HDR);		//Send "StopWD" header to Base Controller
	if(spiTimeout(1000*200) == true){
		spiSendRecvByte(DUMMY);		//Send "PetWD" header to Base Controller
		dataSync();
	}
	return;
}

/*******************************************************************************
Description: Pets the watchdog i.e. resets the timer so it does not trigger. Any
			 P1AM-100 function will pet the watchdog, but this function can be
			 used in long sections of code or slow loops where the other P1AM-100
			 functions are not called.

Parameters: -None

Returns: 	-None
*******************************************************************************/
void P1AM::petWD() {

	spiSendRecvByte(PETWD_HDR);		//Send "PetWD" header to Base Controller
	if(spiTimeout(1000*200) == true){
		spiSendRecvByte(DUMMY);		//Send "PetWD" header to Base Controller
		dataSync();
	}
	return;
}

/*******************************************************************************
Description: Print out current Base Controller firmware version to serial monitor

Parameters: -None

Returns: 	-uint32_t - Firmware Version byte format X.Y.ZZ
*******************************************************************************/
uint32_t P1AM::getFwVersion(){
	uint32_t version = 0;

	if(handleHDR(VERSION_HDR)){
		version = spiSendRecvInt(DUMMY);
		debugPrint("FW V");
		debugPrint(version>>12);
		debugPrint(".");
		debugPrint(version>>8 & 0xF);
		debugPrint(".");
		debugPrintln(version & 0xFF);
		dataSync();
		return version;
	}
	else{
		delay(100);
		return 0;
	}
}

/*******************************************************************************
Description: Flag to see if the Base Controller has been initialised and is running

Parameters: -None

Returns: 	-Bool - Returns true if Base Controller has been intialised and is active
*******************************************************************************/
bool P1AM::isBaseActive(){
	bool isActive = false;
	
	spiSendRecvByte(ACTIVE_HDR);
	if(spiTimeout(1000*200) == true){
	 isActive = spiSendRecvByte(DUMMY);
	}
	dataSync();
	return isActive;
}

/*******************************************************************************
Description: Check to see if any modules that signed on during init have since 
			 dropped out. 

Parameters: -uint8_t numberOfModules - Number of modules expected. Useful if a module
			 never signed on to begin with. If ommitted this function will only
			 check for the number of modules that signed on during the last init
								

Returns: 	-uint8_t - Returns the slot number of the leftmost module in the base
			 that is no longer functioning.
*******************************************************************************/
uint8_t P1AM::checkConnection(uint8_t numberOfModules){
	uint8_t expectedSlots = 0;
	uint16_t activeSlots = 0;
	
	if(!numberOfModules){
		while(baseSlot[expectedSlots].dbLoc != 0){
			expectedSlots++;		//Count number of slots that signed on during init
		}
	}
	else{
		expectedSlots = numberOfModules;
	}
			
	spiSendRecvByte(DROPOUT_HDR);
	if(spiTimeout(1000*200) == true){
		activeSlots  = spiSendRecvByte(DUMMY);		//Get value of active slots. Bitmapped representation.
		activeSlots |= spiSendRecvByte(DUMMY) << 8;
	}
	dataSync();
	
	for(int i=0;i<expectedSlots;i++){
		if((activeSlots >> i) == 0){
			return i+1;	//Leftmost slot with numbering starting at 1
		}
	}
	
	return 0;		//No bad slots found
}

/*******************************************************************************
Label Functions - These use the channelLabel type defined in P1AM.h instead of
passing slot and channel individually. These Turn P1.readDiscrete(1,1) into
P1.readDiscrete(sens_1). They are functionally the same, so if you want to gain
a better understanding of the inner workings, please check out the actual
function code in the above portion of this library.
*******************************************************************************/
uint32_t P1AM::readDiscrete(channelLabel label){
	return readDiscrete(label.slot,label.channel);
}

void P1AM::writeDiscrete(uint32_t data, channelLabel label){
	writeDiscrete(data,label.slot,label.channel);
	return;
}

int P1AM::readAnalog(channelLabel label){
	return readAnalog(label.slot,label.channel);
}

float P1AM::readTemperature(channelLabel label){
	return readTemperature(label.slot,label.channel);
}

void P1AM::writeAnalog(uint32_t data, channelLabel label){
	writeAnalog(data,label.slot,label.channel);
	return;
}

void P1AM::writePWM(float duty, uint32_t freq, channelLabel label){
	writePWM(duty,freq,label.slot,label.channel);
	return;
}

void P1AM::writePWMDuty(float duty, channelLabel label){
	writePWMDuty(duty,label.slot,label.channel);
	return;
}
void P1AM::writePWMFreq(uint32_t freq, channelLabel label){
	writePWMFreq(freq,label.slot,label.channel);
	return;
}

void P1AM::writePWMDir(bool data, channelLabel label){
	writePWMDir(data,label.slot,label.channel);
	return;
}

uint8_t P1AM::checkUnderRange(channelLabel label){
	return checkUnderRange(label.slot,label.channel);
}

uint8_t P1AM::checkOverRange(channelLabel label){
	return checkOverRange(label.slot,label.channel);
}

uint8_t P1AM::checkBurnout(channelLabel label){
	return checkBurnout(label.slot,label.channel);
}

/*******************************************************************************
PRIVATE FUNCTIONS FOR P1AM.h
*******************************************************************************/
void P1AM::dataSync(){
	uint32_t currentMillis = 0;
	uint32_t startMillis = 0;
	
	startMillis = millis();
	while(!digitalRead(slaveAckPin)){
		currentMillis = millis();
		if(currentMillis - startMillis >= 200){
			debugPrintln("Base Sync Timeout");
			break;
		}
	}
	delayMicroseconds(1);
	
	startMillis = millis();
	while(digitalRead(slaveAckPin)){
		currentMillis = millis();
		if(currentMillis - startMillis >= 200){
			debugPrintln("Base Sync Timeout");
			break;
		}
	}
	delayMicroseconds(1);
	
	startMillis = millis();
	while(!digitalRead(slaveAckPin)){
		currentMillis = millis();
		if(currentMillis - startMillis >= 200){
			debugPrintln("Base Sync Timeout");
			break;
		}
	}
	delayMicroseconds(1);
	
	return;
}

char *P1AM::loadConfigBuf(int moduleID){

	switch(moduleID){
		 case 0x34605582:
			return (char*)P1_04AD_1_DEFAULT_CONFIG;
			break;
		case 0x34605583:
			return (char*)P1_04AD_2_DEFAULT_CONFIG;
			break;
		case 0x34605590:
			return (char*)P1_04ADL_2_DEFAULT_CONFIG;
		case 0x34608C8E:
			return (char*)P1_04NTC_DEFAULT_CONFIG;
		case 0x34608C81:
			return (char*)P1_04THM_DEFAULT_CONFIG;
		case 0x34605588:
			return (char*)P1_04RTD_DEFAULT_CONFIG;
		case 0x34605581:
			return (char*)P1_04AD_DEFAULT_CONFIG;
		case 0x3460558F:
			return (char*)P1_04ADL_1_DEFAULT_CONFIG;
		case 0x34A0558A:
			return (char*)P1_08ADL_1_DEFAULT_CONFIG;
		case 0x34A0558B:
			return (char*)P1_08ADL_2_DEFAULT_CONFIG;
		case 0x5461A783:
			return (char*)P1_04ADL2DAL_1_DEFAULT_CONFIG;
		case 0x5461A784:
			return (char*)P1_04ADL2DAL_2_DEFAULT_CONFIG;
		case 0x1403F481:
			return (char*)P1_04PWM_DEFAULT_CONFIG;
		case 0x34A5A481:
			return (char*)P1_02HSC_DEFAULT_CONFIG;
		default:
			break;
	}
	return (char*)P1_04ADL_1_DEFAULT_CONFIG;
}

bool P1AM::handleHDR(uint8_t HDR){

	while(!digitalRead(slaveAckPin));		//Wait for Base Controller to be out of base scanning
	spiSendRecvByte(HDR);					//Send intital Header to ping DMA

	return spiTimeout(MAX_TIMEOUT,HDR,2000);		//1 if we got Base Controller ack, 0 if we took too long
}

uint8_t P1AM::spiSendRecvByte(uint8_t data){

	_P1AM_SPI.begin();
	_P1AM_SPI.beginTransaction(P100_SPI_SETTINGS);
	digitalWrite(slaveSelectPin, LOW);
	data = _P1AM_SPI.transfer(data);
	digitalWrite(slaveSelectPin, HIGH);
	_P1AM_SPI.endTransaction();
	_P1AM_SPI.end();

	return data;
}

uint32_t P1AM::spiSendRecvInt(uint32_t data){
	uint8_t rData[4];
	uint8_t tData[4];
	uint32_t returnInt = 0;

	tData[0] = (data>>0)  & 0xFF;	// data to write to the Base Controller - 1 byte long
	tData[1] = (data>>8)  & 0xFF;	// data to write to the Base Controller - 1 byte long
	tData[2] = (data>>16) & 0xFF;	// data to write to the Base Controller - 1 byte long
	tData[3] = (data>>24) & 0xFF;	// data to write to the Base Controller - 1 byte long

	_P1AM_SPI.begin();
	_P1AM_SPI.beginTransaction(P100_SPI_SETTINGS);
	digitalWrite(slaveSelectPin, LOW);
    rData[0] = _P1AM_SPI.transfer(tData[0]);
    rData[1] = _P1AM_SPI.transfer(tData[1]);
    rData[2] = _P1AM_SPI.transfer(tData[2]);
    rData[3] = _P1AM_SPI.transfer(tData[3]);
    digitalWrite(slaveSelectPin, HIGH);
    _P1AM_SPI.endTransaction();
	_P1AM_SPI.end();

	returnInt  = (rData[3]<<24);
	returnInt += (rData[2]<<16);
	returnInt += (rData[1]<<8);
	returnInt += (rData[0]<<0);

	return returnInt;
}

void P1AM::spiSendRecvBuf(uint8_t *buf, int len, bool returnData){

	_P1AM_SPI.begin();
	_P1AM_SPI.beginTransaction(P100_SPI_SETTINGS);
	digitalWrite(slaveSelectPin, LOW);

	if(returnData){
		for(int i = 0; i < len; ++i){
			buf[i] = _P1AM_SPI.transfer(buf[i]); //return what we get into our buffer
		}
	}
	else{
		for(int i = 0; i < len; ++i){
			_P1AM_SPI.transfer(buf[i]); 		//or don't return what we get
		}
	}

	digitalWrite(slaveSelectPin, HIGH);
	_P1AM_SPI.endTransaction();
	_P1AM_SPI.end();
	return;
}

bool P1AM::spiTimeout(uint32_t uS,uint8_t resendMsg,uint16_t retryPeriod){
	uint16_t retryTime = 0;

	while((!digitalRead(slaveAckPin)) && (uS != 0)){
		delayMicroseconds(1);
		uS--;

		retryTime++;
		if((retryPeriod) && (retryTime == retryPeriod)){		// if we specified a retry period and it equals the time we've waited

			spiSendRecvByte(resendMsg);
			retryTime = 0;
		}
	}
	delayMicroseconds(50);			//small delay to let Base Controller load next msg in buf

	if(uS > 0){
		return 1;
	}
	else{
		debugPrintln("Timeout");
		return 0;
	}
}

bool P1AM::Base_Controller_FW_UPDATE(unsigned int fwLen){
	extern unsigned char FW_IMG_Base_Controller[];
	//int fwLen = sizeof(FW_IMG_Base_Controller);
	int chunkSize = 1000;
	uint8_t status = 0;
	uint8_t tData[9];

	uint8_t *chunkBuffer = (uint8_t *)malloc(chunkSize);	//Make spacs for FW chunks

	_P1AM_SPI.begin();			//Start SPI
	Serial.println("Establishing Communication");

	tData[0] = FW_UPDATE_HDR;

	tData[1] = (fwLen>>0)  & 0xFF;	// data to write to the Base Controller - 1 byte long
	tData[2] = (fwLen>>8)  & 0xFF;	// data to write to the Base Controller - 1 byte long
	tData[3] = (fwLen>>16) & 0xFF;	// data to write to the Base Controller - 1 byte long
	tData[4] = (fwLen>>24) & 0xFF;	// data to write to the Base Controller - 1 byte long

	tData[5] = (chunkSize>>0)  & 0xFF;	// data to write to the Base Controller - 1 byte long
	tData[6] = (chunkSize>>8)  & 0xFF;	// data to write to the Base Controller - 1 byte long
	tData[7] = (chunkSize>>16) & 0xFF;	// data to write to the Base Controller - 1 byte long
	tData[8] = (chunkSize>>24) & 0xFF;	// data to write to the Base Controller - 1 byte long

	spiSendRecvBuf(tData,9);		//send chunk


	if(!spiTimeout(1000*200)){
 		delay(100);
		return 0;
	}
	if(spiSendRecvByte(DUMMY) != FW_UPDATE_HDR){
		delay(100);
		return 0;
	}

	delay(10);
	Serial.println("FW Transfer Started");
	while(!digitalRead(slaveAckPin));			//wait for Base Controller to be ready
	int fullLoops = fwLen/chunkSize;			//How many full chunks

	int offset;
	for ( offset = 0; offset <= fullLoops; offset++)
	{

		Serial.print((float)100*offset/fullLoops,0);//load percentage
		Serial.println("%");

		memcpy(chunkBuffer,(FW_IMG_Base_Controller + offset*chunkSize),chunkSize);	//copy image into buffer to send
		while(!digitalRead(slaveAckPin));			//wait for Base Controller to be ready
		spiSendRecvBuf(chunkBuffer,chunkSize);		//send chunk


	}

	//last chunk
	chunkSize = fwLen % chunkSize;	//get the remaining bytes smaller than 1 chunk
	if(chunkSize != 0)
	{
		memcpy(chunkBuffer,(FW_IMG_Base_Controller + offset*chunkSize),chunkSize);	//copy image into buffer to send
		while(!digitalRead(slaveAckPin));			//wait for Base Controller to be ready
		spiSendRecvBuf(chunkBuffer,chunkSize);
	}

	Serial.println("100%");
	Serial.println("Hang on a sec");

	while(!digitalRead(slaveAckPin));
	status = spiSendRecvByte(DUMMY);	//CRC check on the image


	if(status == 1)
	{
		delay(3000);	//Good, give the Base Controller a few seconds to reboot
		Serial.print("Update Complete! We're now at ");
		getFwVersion();	//Print to confirm new FW is correct version
		return 1;
	}

	else if(status == 4)
	{
		Serial.println("Bad FW Checksum ");
		Serial.println("FW Update FAIL :c");
		return 0;
	}

	else if(status == 5)
	{
		Serial.println("Bad FW Hash ");
		Serial.println("FW Update FAIL :c");
		return 0;
	}

	else if(status == 9)
	{
		Serial.println("Bad FW Hash and CRC");
		Serial.println("FW Update FAIL :c");
		return 0;
	}
	Serial.println("Unknown Error");
	return 0;
}
