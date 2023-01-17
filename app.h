#pragma once
#ifndef _APP_h
#define _APP_h

// C++ libraries
#include <fstream>
#include <iostream>
#include <bitset>
#include <iomanip>
#include <string>
#include <time.h>				// Unix time parsing functions
using namespace std;			// standard c++ library for cout <<

#include <Arduino.h>			// Arduino core library

// Global defines
#define APP "CTM-Demo"
#define REV "1.0.0"			// Update this number when the version is changed

#define DEBUG				// Compile code needed for debugging

#define MAX_BUFF 6			// size of LCD  buffer

#define SEALEVELPRESSURE_HPA (1013.25)


// Sensor type enumarator used in mySensors class
enum SENSOR_TYPES {
	NO_SENSOR,
	BME280_SENSOR,
	HDC2080_SENSOR
};

// Enumerator for font size on the LCD
enum font_sizes {
	SMALL,
	MEDIUM,
	LARGE
};


// Global structure for time values
struct timeVals {
	time_t local;
	time_t UTC;
	String strLocal;
	String strUTC;
};

// Sensor data value structures, holding float and formated string
struct dataVals {
	float floatVal = 0;
	String strVal = "0";
};

// Formated data values of all sensors, each value folds float and string
struct dataPointVals {
	dataVals Temperature;
	dataVals Temperature_F;
	dataVals Humidity;
	dataVals Pressure;
	dataVals Pressure_Bar;
	dataVals DewPoint;
	dataVals DewPoint_F;
	dataVals Altitude;
};

// Raw sensor values and time of log
struct dataStruct {
	time_t logTime;
	float Temperature;
	float Humidity;
	float Pressure;
	float DewPoint;
};


// Special Arduino libraries
#include <Wire.h>               // I2C function
#include <SSD1306Wire.h>        // SSD1306 LCD library

// Application Classes
#include "mySensors.h"			// All sensor operations
#include "myLCD.h"				// LCD operations
#include "fonts.h"				// Bits of fonts

#include "MS5607.h"				// MS5607 chip:  pressure and temp
#include "HDC2080.h"			// HDC2080 chip: temp and humidity


#endif
