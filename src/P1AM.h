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

  P1AM.h - Library for Communicating with P1000 Series Modules
*/

#ifndef P1AM_h
#define P1AM_h

#include "Arduino.h"
#include "SPI.h"
#include "Module_List.h"
#include "defines.h"

struct channelLabel{			//Used to call functions through names rather than slot/channel numbers.
	uint8_t slot;
	uint8_t channel;
};

class P1AM{
	public:
	P1AM();

	//Init
	uint8_t init();	//Initialise modules in the base. Returns the number of slots that have signed on.
	void enableBaseController(bool state);	//Enable or Disable base controller. Automatically called in init.
	uint16_t rollCall(const char* moduleNames[], uint8_t numberOfModules);		//Pass in an array of module names to check if current modules in base match.

	//Data IO Functions	- For more info see function headers in P1AM.cpp
	uint32_t readDiscrete(uint8_t slot, uint8_t channel = 0);					//Read Discrete Module. Passing 0 instead of a channel will return data from all of the channels at once.
	void writeDiscrete(uint32_t data,uint8_t slot, uint8_t channel = 0);		//Write Discrete Module. Passing 0 instead of a channel will write data for all of the channels at once.
	int readAnalog(uint8_t slot, uint8_t channel);								//Read Analog Module. Returns 32 bits of data. 16/14/12/etc bit modules are not scaled and will return a bit appropriate value.
	float readTemperature(uint8_t slot, uint8_t channel);						//Read Temperature Module. Returns float.
	void writeAnalog(uint32_t data,uint8_t slot, uint8_t channel);				//Write Analog Module. Send up to 32 bits of data. 16/14/12/etc bit modules are masked on Base Controller
	void readBlockData(char *buf, uint16_t len,uint16_t offset, uint8_t type);	//Read raw  data buffers. Allows for data updates for large numbers of points.
	void writeBlockData(char *buf, uint16_t len,uint16_t offset, uint8_t type); //Write to raw data buffers. Allows for data updates for large numbers of points.

	//PWM Module functions - For more info see function headers in P1AM.cpp
	void writePWM(float duty,uint32_t freq,uint8_t slot,uint8_t channel);		//Set duty cycle and frequency of a PWM module channel
	void writePWMDuty(float duty,uint8_t slot,uint8_t channel);					//Set duty cycle of a PWM module channel without changing its frequency
	void writePWMFreq(uint32_t freq,uint8_t slot,uint8_t channel);				//Set frequency of a PWM module channel without changing its duty cycle
	void writePWMDir(bool data,uint8_t slot, uint8_t channel);					//Set state of PWM channel when configured for DIR mode

	//Utility Functions - For more info see function headers in P1AM.cpp
	uint8_t printModules();									//Print list of all signed on modules to the Serial monitor
	uint8_t checkUnderRange(uint8_t slot, uint8_t channel = 0);//Returns 1 if slot channel is under range and supports this function
	uint8_t checkOverRange(uint8_t slot, uint8_t channel = 0);//Returns 1 if slot channel is over range and supports this function
	uint8_t checkBurnout(uint8_t slot, uint8_t channel = 0);//Returns 1 if slot channel is in burnout and supports this function
	uint8_t check24V(uint8_t slot);							//Returns 1 if slot is missing 24V and supports this status
	char readStatus(int byteNum,int slot);  				//Return 1 status byte for a module in a slot. See !!!!! THIS DOC REFERENCED !!!!!! for details
	void readStatus(char buf[], uint8_t slot);				//Return all status bytes for a module in a slot. See !!!!! THIS DOC REFERENCED !!!!!! for details
	bool configureModule(char cfgData[],uint8_t slot);		//Select slot and pass in buffer that contains configuration data see !!!!! THIS DOC REFERENCED !!!!!! for details
	bool configureModule(const char cfgData[], uint8_t slot);
	void readModuleConfig(char cfgData[], uint8_t slot);	//Select slot and pass in buffer to return configurationd data to. Does not indicate config is live
	void configWD(uint16_t milliseconds, uint8_t toggle);	//Configure Watchdog Behaviour
	void startWD();											//Start Watchdog Functionality
	void stopWD();											//Stop Watchdog Functionality
	void petWD();											//Pet WD in case you aren't talking to Base Controller often
	uint32_t getFwVersion();								//Returns the Base Controller FW version
	bool isBaseActive();									//Check if Base Controller is currently configured. If not, call init.
	uint8_t checkConnection(uint8_t numberOfModules = 0);	//Checks the modules to see if a connection has been lost. Returns first missing module.
	bool Base_Controller_FW_UPDATE(unsigned int fwLen);		//For FW update of Base Controller
	moduleProps readSlotProps(uint8_t slot);                //Returns the module properties at the given slot location.
	
	//Label functions - functionally the same as the above Data IO but use the channelLabel datatype for easier to read code.
	uint32_t readDiscrete(channelLabel label);
	void writeDiscrete(uint32_t data, channelLabel label);
	int readAnalog(channelLabel label);
	float readTemperature(channelLabel label);
	void writeAnalog(uint32_t data, channelLabel label);
	void writePWM(float duty,uint32_t freq, channelLabel label);
	void writePWMDuty(float duty, channelLabel label);
	void writePWMFreq(uint32_t freq, channelLabel label);
	void writePWMDir(bool data, channelLabel label);
	uint8_t checkUnderRange(channelLabel label);
	uint8_t checkOverRange(channelLabel label);
	uint8_t checkBurnout(channelLabel label);

	//Private functions for Base Controller communication.
	private:
	uint8_t spiSendRecvByte(uint8_t data);
	uint32_t spiSendRecvInt(uint32_t data);
	void spiSendRecvBuf(uint8_t *buf, int len,  bool returnData = 0);
	bool spiTimeout(uint32_t uS, uint8_t resendMsg = 0,uint16_t retryPeriod = 0);
	char *loadConfigBuf(int moduleID);
	bool  handleHDR(uint8_t HDR);
	void dataSync();
	struct moduleInfo{
		uint8_t dbLoc;			//mdb location
	}baseSlot[NUMBER_OF_MODULES];

};

extern P1AM P1;	//Default P1AM class instance

#endif
