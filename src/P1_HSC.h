/*
MIT License

Copyright (c) 2019 FACTS Engineering, LLC

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

#ifndef P1_HSC_h
#define P1_HSC_h

#include "P1AM.h"

#define oneZ 1
#define threeIn 2
#define twoZ 3
#define fourIn 4

#define stepDirection 0
#define quad4x  1
#define quad1x  2

#define negativeDirection 0
#define positiveDirection 1

class P1_HSC_Channel{

	public:
	P1_HSC_Channel(uint8_t slotSelect = 0, uint8_t channelSelect = 0);

	uint8_t channelNumber = 0;
	uint8_t slotNumber = 0;

	//Configuration Registers - See above defines for settings
	bool isRotary = false;
	bool enableZReset = false;
	uint8_t inhibitOn = false;
	uint8_t mode = stepDirection;
	bool polarity = positiveDirection;


	//Reading Functions
	int readPosition(void); 	//Returns the current position
	uint32_t readAlerts(void);	//Returns alert status bits of channel
	int readRollOver(void);		//Function to be used for polling. Returns 1 for over,-1 for under, 0 for no rollover
	uint8_t readInputs(void);	//Read the 4 inputs that correspond to the channel

	//Writing Functions
	void setPosition(int counts);			//Sets current position to this value
	void setZResetValue(int counts);		//Sets the value to use for Z-reset
	void setRolloverPosition(int counts);	//Sets the roll-over position for rotary encodeers

	//Default rollover
	int rolloverPosition = 0x7FFFFFFF;	//Default to max

};

class P1_HSC_Module{

	public:
	P1_HSC_Module(uint8_t slotSelect);
	P1_HSC_Channel CNT1;	//Default instance of Channel 1
	P1_HSC_Channel CNT2;	//Default instance of Channel 2

	//Discrete inputs
	uint8_t inputStatus = 0;
	bool status1A;
	bool status1B;
	bool status1Z;
	bool status3IN;
	bool status2A;
	bool status2B;
	bool status2Z;
	bool status4IN;

	//User Functions
	uint8_t readInputs(void);		//Return bitmapped output and updates the status bool properties
	void configureChannels(void);	//intialises module with configuration of CNT1 and CNT2
	void configureChannels(P1_HSC_Channel CH1, P1_HSC_Channel CH2);	//Init option for custom channel class instances

	private:
	uint8_t slotNumber = 0;
	bool checkIfHSC(uint8_t slot);
};

#endif
