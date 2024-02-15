/*MIT License

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

#ifndef defines_h
#define defines_h

#define DEBUG_PRINT_ON		//Comment this out to stop debug messages from being printed to the Serial console. This only will affect messages from this library.

#ifdef DEBUG_PRINT_ON
	#define debugPrintln(a) (Serial.println(a))
	#define debugPrint(a) (Serial.print(a))
#else
	#define debugPrintln(a)//nothing, disables printing from this function
	#define debugPrint(a) //nothing, disables printing from this function
#endif


#ifdef _VARIANT_P1AM_200
	#define NUMBER_OF_MODULES	15 
	#define slaveSelectPin 		44
	#define slaveAckPin 		47
	#define SWITCH_BUILTIN		31
	#define baseEnable			46
	#define _P1AM_SPI 	SPI2
#else
	#define NUMBER_OF_MODULES	15 
	#define slaveSelectPin 		A3
	#define slaveAckPin 		A4
	#define SWITCH_BUILTIN		31
	#define baseEnable			33
	#define _P1AM_SPI 	SPI
#endif



#define MOD_HDR 			0x02
#define VERSION_HDR 		0x03
#define ACTIVE_HDR			0x04
#define DROPOUT_HDR			0x05
#define CFG_HDR				0x10
#define READ_CFG_HDR 		0x11
#define PETWD_HDR			0x30
#define STARTWD_HDR			0x31
#define STOPWD_HDR			0x32
#define CONFIGWD_HDR		0x33
#define READ_STATUS_HDR 	0x40
#define READ_DISCRETE_HDR 	0x50
#define READ_ANALOG_HDR		0x51
#define READ_BLOCK_HDR		0x52
#define WRITE_DISCRETE_HDR 	0x60
#define WRITE_ANALOG_HDR	0x61
#define WRITE_BLOCK_HDR		0x62
#define FW_UPDATE_HDR		0xAA

#define DUMMY		0xFF
#define EMPTY_SLOT_ID 0xFFFFFFFE
#define MAX_TIMEOUT	  0xFFFFFFFF

#define DISCRETE_IN_BLOCK	0
#define ANALOG_IN_BLOCK		1
#define DISCRETE_OUT_BLOCK	2
#define ANALOG_OUT_BLOCK	3
#define STATUS_IN_BLOCK		4

#define MISSING24V_STATUS 	3
#define BURNOUT_STATUS		5
#define UNDER_RANGE_STATUS	7
#define OVER_RANGE_STATUS	11

#define TOGGLE				0x01
#define HOLD				0x00

//#define AUTO_CONFIG_OFF			//Automatically configure modules with defaults. Recommended to keep this undefined as you can re-configure manually later.

#endif
