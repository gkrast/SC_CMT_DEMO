/* The Public License
 ******************************************************************************
 *                                                                            *
 * Copyright (c) 2021 by Senzotic, George Krastev                             *
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

#include "MS5607.h"
MS5607Class MS5607;

//Initializations
bool MS5607Class::init() {
	this->_address = MS5607_SLAVE_ADDRESS;
	Wire.begin(SDA, SCL);
	Wire.setClock(70000);
	//Send Reset command
	Wire.beginTransmission(_address);
	Wire.write(CMD_RESET);
	Wire.endTransmission(true);
	delay(10); 
	//Read coeficients after  reset
	return (getPROM()) ? true : false;
};

// Read the configuration data from PROM
bool MS5607Class::getPROM() {
	uint8_t k = 0;
	uint16_t CRC4;
	for (k = 0; k < 8; k ++)
	{
		Wire.beginTransmission(_address);
		Wire.write(CMD_READ_PROM + k*2);
		Wire.endTransmission(false);
		Wire.requestFrom(_address, (size_t)2, true);
		C[k] = Wire.read()<<8;
		C[k] += Wire.read();
	}
	CRC4 = getCRC4(C); // Get CRC code

	//// Test code
	/*cout << setfill('0') << setw(6) << uppercase << hex;
	cout << "RESERVED: " << C[0] << endl;
	cout << "SENS: " << C[1] << endl;
	cout << "OFF: " << C[2] << endl;
	cout << "TCS: " << C[3] << endl;;
	cout << "TCO: " << C[4] << endl;;
	cout << "TREF: " << C[5] << endl;
	cout << "TEMPSENS: " << C[6] << endl;
	cout << "CRC: " << C[7] << endl;
	cout << "CALC. CRC: " << static_cast<int>(CRC4) << endl;
	cout << "-----------------------------------" << endl;*/
	
	return ((C[7] & 0x000F) == CRC4)? true: false; // Data integrity check
};

// Calculate 4-bit CRC from PROM data array
uint8_t MS5607Class::getCRC4(uint16_t n_prom[])
{
	int cnt;				// simple counter
	uint16_t  n_rem;		// crc reminder
	uint16_t  crc_read;	// original value of the crc
	uint8_t  n_bit;
	n_rem = 0x00;
	crc_read = n_prom[7];   //save read CRC
	n_prom[7] = (0xFF00 & (n_prom[7])); //CRC byte is replaced by 0
	for (cnt = 0; cnt < 16; cnt++) // operation is performed on bytes
	{// choose LSB or MSB
		if (cnt % 2 == 1) n_rem ^= (uint16_t)((n_prom[cnt >> 1]) & 0x00FF);
		else n_rem ^= (uint16_t)(n_prom[cnt >> 1] >> 8);
		for (n_bit = 8; n_bit > 0; n_bit--)
		{
			if (n_rem & (0x8000))
			{
				n_rem = (n_rem << 1) ^ 0x3000;
			}
			else
			{
				n_rem = (n_rem << 1);
			}
		}
	}
	n_rem = (0x000F & (n_rem >> 12)); // final 4-bit reminder is CRC code
	n_prom[7] = crc_read; // restore the crc_read to its original place
	return (n_rem ^ 0x0);
}


//Read Pressure and Temperature
bool MS5607Class::getData(float* pressure) {
	uint8_t ACK;
	int ctr = 0;
	unsigned long D1;	// ADC value of the pressure conversion
	unsigned long D2;	// ADC value of the temperature conversion	
	float P;			// compensated pressure value
	float T;			// compensated temperature value
	double dT;			// difference between actual and measured temperature
	double OFF;			// offset at actual temperature
	double SENS;		// sensitivity at actual temperature

	// Second order adjustments
	float T2=0;		// compensated temperature value
	double OFF2=0;		// offset at actual temperature
	double SENS2=0;		// sensitivity at actual temperature
	
	// ADC on pressure
	Wire.beginTransmission(_address);
	Wire.write(CMD_CONVERT_PRESSURE_4096);
	Wire.endTransmission(true);
	delay(10);
	do {
		Wire.beginTransmission(_address);
		Wire.write(CMD_READ_ADC);
		ACK=Wire.endTransmission(true);
		ctr++;
		if (ctr == 100) return false; // time out error
	} while (ACK != 0);
	Wire.requestFrom(_address, (size_t)3, true);
	D1 = Wire.read()<<16;
	D1 += Wire.read()<<8;
	D1 += Wire.read();

	//ADC on temperature
	ctr = 0;
	Wire.beginTransmission(_address);
	Wire.write(CMD_CONVERT_TEMP_4096);
	Wire.endTransmission(true);
	delay(10);
	do {
		Wire.beginTransmission(_address);
		Wire.write(CMD_READ_ADC);
		ACK = Wire.endTransmission(false);
		ctr++;
		if (ctr == 100) return false; // time out error
	} while (ACK != 0);
	Wire.requestFrom(_address, (size_t)3, true);
	D2 = Wire.read()<<16;
	D2 += Wire.read()<<8;
	D2 += Wire.read();

	//Calculate first order of Temparature and Pressure

	dT = D2 - C[5] * pow((double)2, (double)8);
	OFF = C[2] * pow((double)2, (double)17) + dT * C[4] / pow((double)2, (double)6);
	SENS = C[1] * pow((double)2, (double)16) + dT * C[3] / pow((double)2, (double)7);

	T = (2000 + (dT * C[6]) / pow((double)2, (double)23)) / 100;
	
	// Second order compensation for nonlinearity,which takes place below 20C and below -15C
	if (T < 20) {
		T2 = pow(dT, 2) / pow((double)2, (double)31);
		OFF2 = 61 * pow(T - 2000, 2) / 16;
		SENS2 = 2 * (pow(T - 2000, 2));
		if (T < -15) {
			OFF2 = OFF2 + 15 * pow(T + 1500, 2);
			SENS2 = SENS2 + 8 * pow(T + 1500, 2);
		}
	}

	//Compensate calculated values
	T = T - T2;
	OFF = OFF - OFF2;
	SENS = SENS - SENS2;

	P = (((D1 * SENS) / pow((double)2, (double)21) - OFF) / pow((double)2, (double)15)) / 100;
	
	*pressure = P;

	//// Test code
	//cout << "PRESSURE ADC: " << D1 << endl;
	//cout << "TEMP ADC: " << D2 << endl;
	//cout << "T: " << T << endl;
	//cout << "P: " << P << endl;
	//cout << "-----------------------------------" << endl;

	return true;
};