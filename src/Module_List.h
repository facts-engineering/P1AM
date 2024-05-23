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

#ifndef Module_List_h
#define Module_List_h

const struct moduleProps
{
	unsigned int moduleID;
	char diBytes;			//Number of bytes used by all Discrete Input channels
	char doBytes;			//Number of bytes used by all Discrete Output channels
	char aiBytes;			//Number of bytes used by all Analog Input channels
	char aoBytes;			//Number of bytes used by all Analog Output channels
	char statusBytes;		//Number of status bytes. Things like overrange errors or missing 24V.
	char configBytes;		//Number of bytes used to configure module. Things like ranges and enabled channels.
	char dataSize;			//Resolution or Specialty info
	const char* moduleName;	//Text name of module
}mdb[] = {

  //{0x000000ID,di,do,ai,ao,st,cf,ds}
    {0x00000000, 0, 0, 0, 0, 0, 0, 0, "Empty"}, //Empty first entry for defaults

	{0x04A00042, 1, 0, 0, 0, 0, 0, 1, "P1-08ND-TTL"}, //P1-08ND-TTL

	{0x04A00081, 1, 0, 0, 0, 0, 0, 1, "P1-08ND3"},	//P1-08ND3

	{0x04A00085, 1, 0, 0, 0, 0, 0, 1, "P1-08NA"},	//P1-08NA

	{0x04A00087, 1, 0, 0, 0, 0, 0, 1, "P1-08SIM"},	//P1-08SIM

	{0x04A00088, 1, 0, 0, 0, 0, 0, 1, "P1-08NE3"},	//P1-08NE3

	{0x05200082, 2, 0, 0, 0, 0, 0, 1, "P1-16ND3"},	//P1-16ND3

	{0x05200089, 2, 0, 0, 0, 0, 0, 1, "P1-16NE3"},	//P1-16NE3

	{0x14030050, 0, 1, 0, 0, 0, 0, 1, "P1-04TRS"},	//P1-04TRS
    
    {0x1403F481, 0, 0, 0, 32, 4, 4, 0xA0, "P1-04PWM"},	//P1-04PWM

	{0x1404008D, 0, 1, 0, 0, 0, 0, 1, "P1-08TA"},	//P1-08TA

	{0x1404008F, 0, 1, 0, 0, 0, 0, 1, "P1-08TRS"},	//P1-08TRS

	{0x14040091, 0, 2, 0, 0, 0, 0, 1, "P1-16TR"},	//P1-16TR

	{0x14050046, 0, 1, 0, 0, 0, 0, 1, "P1-08TD-TTL"}, //P1-08TD-TTL

	{0x14050081, 0, 1, 0, 0, 0, 0, 1, "P1-08TD1"},	//P1-08TD1

	{0x14050082, 0, 1, 0, 0, 0, 0, 1, "P1-08TD2"},	//P1-08TD2

	{0x14080085, 0, 2, 0, 0, 0, 0, 1, "P1-15TD1"},	//P1-15TD1

	{0x14080086, 0, 2, 0, 0, 0, 0, 1, "P1-15TD2"},	//P1-15TD2

	{0x24A50081, 1, 1, 0, 0, 0, 0, 1, "P1-16CDR"},	//P1-16CDR

	{0x24A50082, 1, 1, 0, 0, 0, 0, 1, "P1-15CDD1"},	//P1-15CDD1

	{0x24A50083, 1, 1, 0, 0, 0, 0, 1, "P1-15CDD2"},	//P1-15CDD2

	{0x34605581, 0, 0, 16, 0, 12, 18, 16, "P1-04AD"},	//P1-04AD

    {0x34605582, 0, 0, 16, 0, 12, 2, 16, "P1-04AD-1"},	//P1-04AD-1

	{0x34605583, 0, 0, 16, 0, 12, 2, 16, "P1-04AD-2"},	//P1-04AD-2

	{0x34605588, 0, 0, 16, 0, 12, 8, 16, "P1-04RTD"},	//P1-04RTD

	{0x3460558F, 0, 0, 16, 0, 12, 2, 12, "P1-04ADL-1"}, //P1-04ADL-1

	{0x34605590, 0, 0, 16, 0, 12, 2, 12, "P1-04ADL-2"}, //P1-04ADL-2

	{0x34608C81, 0, 0, 16, 0, 12, 20, 32, "P1-04THM"},	//P1-04THM

	{0x34608C8E, 0, 0, 16, 0, 12, 8, 32, "P1-04NTC"}, 	//P1-04NTC

	{0x34A0558A, 0, 0, 32, 0, 12, 2, 12, "P1-08ADL-1"}, //P1-08ADL-1

	{0x34A0558B, 0, 0, 32, 0, 12, 2, 12, "P1-08ADL-2"}, //P1-08ADL-2

	{0x34A5A481, 2, 0, 36, 36, 4, 12, 0xC0, "P1-02HSC"}, //P1-02HSC

	{0x44035583, 0, 0, 0, 16, 4, 0, 12, "P1-04DAL-1"},	//P1-04DAL-1

	{0x44035584, 0, 0, 0, 16, 4, 0, 12, "P1-04DAL-2"}, //P1-04DAL-2

	{0x44055588, 0, 0, 0, 32, 4, 0, 12, "P1-08DAL-1"}, //P1-08DAL-1

	{0x44055589, 0, 0, 0, 32, 4, 0, 12, "P1-08DAL-2"}, //P1-08DAL-2

	{0x5461A783, 0, 0, 16, 8, 12, 2, 12, "P1-4ADL2DAL-1"}, //P1-4ADL2DAL-1

	{0x5461A784, 0, 0, 16, 8, 12, 2, 12, "P1-4ADL2DAL-2"}, //P1-4ADL2DAL-2

	{0xFFFFFFFF, 0, 0, 0, 0, 0, 0, 0, "BAD SLOT"}, //empty in case no modules are defined.

	{0x00000000, 0, 0, 0, 0, 0, 0, 0, "BAD SLOT"} //empty in case no modules are defined.
};

const char P1_04AD_1_DEFAULT_CONFIG[] = {0x40,0x03};

const char P1_04AD_2_DEFAULT_CONFIG[] = {0x40,0x03};

const char P1_04ADL_1_DEFAULT_CONFIG[] = {0x40,0x03};

const char P1_04ADL_2_DEFAULT_CONFIG[] = {0x40,0x03};

const char P1_08ADL_1_DEFAULT_CONFIG[] = {0x40,0x07};

const char P1_08ADL_2_DEFAULT_CONFIG[] = {0x40,0x07};

const char P1_04PWM_DEFAULT_CONFIG[] = {0x02,0x02,0x02,0x02};

const char P1_04ADL2DAL_1_DEFAULT_CONFIG[] = {0x40,0x03};

const char P1_04ADL2DAL_2_DEFAULT_CONFIG[] = {0x40,0x03};

const char P1_04NTC_DEFAULT_CONFIG[] = {0x40,0x03,0x60,0x05,
										0x20,0x00,0x80,0x02};

const char P1_04THM_DEFAULT_CONFIG[] = {0x40,0x03,0x60,0x05,
										0x21,0x00,0x22,0x00,
										0x23,0x00,0x24,0x00,
				       						0x00,0x00,0x00,0x00,
				       						0x00,0x00,0x00,0x00};

const char P1_04RTD_DEFAULT_CONFIG[] = {0x40,0x03,0x60,0x05,
										0x20,0x01,0x80,0x00};

const char P1_04AD_DEFAULT_CONFIG[] = 	{0x40,0x03,0x00,0x00,
										 0x20,0x03,0x00,0x00,
										 0x21,0x03,0x00,0x00,
										 0x22,0x03,0x00,0x00,
										 0x23,0x03};
										 

const char P1_02HSC_DEFAULT_CONFIG[] = {0x00,0x00,0x00,0x00,
										0x00,0x00,0x00,0x01,
										0x00,0x00,0x00,0x01};



#endif
