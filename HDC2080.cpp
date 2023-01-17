/* The Public License
 ******************************************************************************
 *                                                                            *
 * Copyright (c) 2023 by Senzotic, George Krastev                             *
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

#include "HDC2080.h"

 //----------- Global Vars
extern bool Debug; // Enable dispaly of debug info

HDC2080Class HDC2080;

bool HDC2080Class::init()
{
	uint16_t DevID;
	this->_address = HDC2080_SLAVE_ADDRESS;
	Wire.begin(SDA, SCL);
	Wire.setClock(70000);
	reset();
	DevID = getDeviceID();
	////// Test code
	//cout << setfill('0') << setw(4) << uppercase << hex;
	//cout << "HDC2080 Device ID: " << DevID << endl;
	//cout << "-----------------------------------" << endl;
	////// end test
	return (DevID == 0x07D0) ? true : false;
}

//Soft reset
void HDC2080Class::reset() {
	uint8 config_reg;
	float t, h;
	Wire.beginTransmission(_address);
	Wire.write(RESET_DRDY_INT_CONF);
	Wire.endTransmission(false);
	Wire.requestFrom(_address, (size_t)1,true);
	config_reg = Wire.read();
	config_reg |= SOFT_RESET;
	Wire.beginTransmission(_address);
	Wire.write(RESET_DRDY_INT_CONF);
	Wire.write(config_reg);
	Wire.endTransmission(true);
	delay(10);
	getData(&t, &h);
};

//Read Temperature and Humidity sensors
bool HDC2080Class::getData(float* temp, float* rh) {
	uint16_t _temp;
	uint16_t _rh;
	uint8_t config_reg = 1 + (_rh_resolution << 4) + (_temp_resolution << 6);
	int delay_ctr = 0;
	
	//Trigger measurement
	Wire.beginTransmission(_address);
	Wire.write(MEASUREMENT_CONFIGURATION);
	Wire.write(config_reg);
	Wire.endTransmission(true);

	//Check when the measurement is completed
	while (config_reg & 1){		
		Wire.beginTransmission(_address);
		Wire.write(MEASUREMENT_CONFIGURATION);
		Wire.endTransmission(false);
		Wire.requestFrom(_address, (size_t)1, false);	
		config_reg = Wire.read();
		if (config_reg & 1) {
			delay(10);
			delay_ctr++;
			if (delay_ctr > 10) return false; // time out error
		}
	}

	//Read the measurement result
	Wire.beginTransmission(_address);
	Wire.write(TEMPERATURE_LOW);
	Wire.endTransmission(false);
	Wire.requestFrom(_address, (size_t)4,true);
	_temp = Wire.read();
	_temp += Wire.read()<<8;
	_rh = Wire.read();
	_rh += Wire.read()<<8;

	//// Test code
	//cout << setfill('0') << setw(4) << uppercase << hex;
	//cout << endl << "Raw_Temp: " << _temp << endl;
	//cout << "Raw_Rh: " << _rh << endl;
	//cout << "-----------------------------------" << endl;

	//Calculate final values and return results
	*temp =(_temp / pow((double)2,16)) * 165 - 40;
	*rh = (_rh / pow((double)2, 16)) * 100;
	
	return true;
};

//Offsets Temperature Reading
bool HDC2080Class::offsetTemp(float offset) {
	int8_t temp_offset_reg;
	if (offset > 20.46) {
		return false; // overrange
	} 
	else if(offset < -20.62){
		return false; // overrange
	} 
	else  {
		temp_offset_reg = offset / 0.16;
	}	
	// Update the chip
	Wire.beginTransmission(_address);
	Wire.write(TEMP_OFFSET_ADJUST);
	Wire.write(temp_offset_reg);
	Wire.endTransmission(true);
	return true;
};

//Offsets Humidity Reading
bool HDC2080Class::offsetRH(float offset) {
	int8_t rh_offset_reg;
	if (offset > 24.9) {
		return false; // overrange
	}
	else if (offset < -25) {
		return false; // overrange
	}
	else {
		rh_offset_reg = offset / 0.2;
	}	
	// Update the chip
	Wire.beginTransmission(_address);
	Wire.write(HUM_OFFSET_ADJUST);
	Wire.write(rh_offset_reg);
	Wire.endTransmission(true);
	return true;
};

//Sets Temperature resolution
void HDC2080Class::setTempRes(Resolution res) {
	this->_temp_resolution = res;
};

//Sets Humidity resolution
void HDC2080Class::setRHRes(Resolution res) {
	this->_rh_resolution=res;
};

//Gets Device ID
uint16_t HDC2080Class::getDeviceID() {
	uint16_t dev_id;
	Wire.beginTransmission(_address);
	Wire.write(DEVICE_ID_LOW);
	Wire.endTransmission(false);
	Wire.requestFrom(_address, (size_t)2,true);
	dev_id = Wire.read();
	dev_id += Wire.read()<<8;
	return dev_id;
};

//Internal heater on for 10 sec (blocking)
void HDC2080Class::heater(bool on) {
	uint8 config_reg;
	Wire.beginTransmission(_address);
	Wire.write(RESET_DRDY_INT_CONF);
	Wire.endTransmission(false);
	Wire.requestFrom(_address, (size_t)1,true);
	config_reg = Wire.read();
	if (on) {
		config_reg |= HEAT_EN;
		Wire.beginTransmission(_address);
		Wire.write(RESET_DRDY_INT_CONF);
		Wire.write(config_reg);
		Wire.endTransmission(true);
	}
	else {
		config_reg &= ~((uint8_t)HEAT_EN);
		Wire.beginTransmission(_address);
		Wire.write(RESET_DRDY_INT_CONF);
		Wire.write(config_reg);
		Wire.endTransmission(true);
	}
};



