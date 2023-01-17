/* The Public License
 ******************************************************************************
 *                                                                            *
 * Copyright (c) 2023 by Senzotic					                          *
 *                                                                            *
 ******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _MS5607_h
#define _MS5607_h

#include "app.h"

#define MS5607_SLAVE_ADDRESS  0b1110110

 //MS5607 Commands
#define CMD_RESET 0x1E
#define CMD_CONVERT_PRESSURE_4096 0x48
#define CMD_CONVERT_TEMP_4096 0x58
#define CMD_READ_ADC 0x0
#define CMD_READ_PROM 0xA0

class MS5607Class
{
public:
	//Initializations
	bool init();

	//read PROM
	bool getPROM();

	//Read Pressure and Temperature
	bool getData(float* pressure);


protected:
	uint8_t _address;
	uint16_t C[8];;		//Calibartion Data
	uint8_t getCRC4(uint16_t n_prom[]); // 4-bit CRC calculator
};

extern MS5607Class MS5607;


#endif

