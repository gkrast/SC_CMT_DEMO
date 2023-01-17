 /* The Public License
 ******************************************************************************
 *                                                                            *
 * Copyright (c) 2023 by Senzotic											  *
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

#ifndef _HDC2080_h
#define _HDC2080_h

#include "app.h"

#define HDC2080_SLAVE_ADDRESS  0b1000000

//HDC2080 Registers
#define TEMPERATURE_LOW 0x00
#define TEMPERATURE HIGH 0x01
#define HUMIDITY_LOW 0x02
#define HUMIDITY_HIGH 0x03
#define INTERRUPT_DRDY 0x04
#define TEMPERATURE_MAX 0x5
#define HUMIDITY_MAX 0x06
#define INTERRUPT_ENABLE 0x07
#define TEMP_OFFSET_ADJUST 0x08
#define HUM_OFFSET_ADJUST 0x09
#define TEMP_THR_L 0x0A
#define TEMP_THR_H 0x0B
#define RH_THR_L 0x00C
#define RH_THR_H 0x0D
#define RESET_DRDY_INT_CONF 0x0E
#define MEASUREMENT_CONFIGURATION 0x0F

#define MANUFACTURER_ID_LOW 0xFC
#define MANUFACTURER_ID_HIGH 0xFD
#define DEVICE_ID_LOW 0xFE
#define DEVICE_ID_HIGH 0xFF

// Bit masks for Config Register 0x0E
#define HEAT_EN		0b00001000 // set heater on 
#define SOFT_RESET	0b10000000 // software reset bit

// Reading resolution settings
enum Resolution {
	_14BIT = 0,
	_11BIT = 1,
	_9BIT = 2
};

class HDC2080Class
{
public:
	//Initializations
	bool init();

	//Read Temperature and Humidity
	bool getData(float* temp, float* rh);

	//Offsets temparature reading
	bool offsetTemp(float offset);

	//Set Humidity resolution
	void setTempRes(Resolution res);


	//Offsets Humidity reading
	bool offsetRH(float offset);

	//Set Humidity resolution
	void setRHRes(Resolution res);

	//Get Device ID
	uint16_t getDeviceID();

	//internal heater on for 1 sec
	void heater(bool on);
    
	//reset the chip after powerup
	void reset();

protected:
	uint8_t _address;
	uint8_t	_rh_resolution = 0;		//14-bit default
	uint8_t	_temp_resolution = 0;	//14-bit default
};

extern HDC2080Class HDC2080;

#endif

