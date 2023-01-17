#include "mySensors.h"

MySensorClass Sensors;

// Sensor Initializations
bool MySensorClass::init() {
	int k;
	sensorsConnected = true;	
	if(HDC2080.init()){
		//Board with HDC2080 and MS5607 sensor pair detected
		LCD.showText("HDC2080 Sensor: OK");
		cout << "HDC2080 Sensor: OK" << endl;
		sensorTypes = HDC2080_SENSOR;
		if (!MS5607.init()) {
			//Handle Presure sensor Error
			LCD.showText("MS5607 Not Found!");
			cout << "MS5607 Not Found!" << endl;			
		}
		else {
			LCD.showText("MS5607 Sensor: OK");
			cout << "MS5607 Sensor: OK" << endl;
		}

	}
	else {
		sensorsConnected = false; // no sensors detected
		LCD.showText("No Sensors!");
		cout << "No Sensors!" << endl;
		return false;
	}
	return true;
}


// Read data for real time display
dataPointVals MySensorClass::readData() {

	dataPointVals _dataPoint;
	

	// Temperature, Humidity and Pressure sensors
	switch (sensorTypes){
		case HDC2080_SENSOR:
			if (!HDC2080.getData(&_dataPoint.Temperature.floatVal, &_dataPoint.Humidity.floatVal)) {
				//Handle Time out sensor Error
				LCD.showText("HDC2080 Timeout Error!");
			};
			if (!MS5607.getData(&_dataPoint.Pressure.floatVal)) {
				//Handle Time out sensor Error
				LCD.showText("MS5607 Timeout Error!");
			}
		    break;
		//<<<<<<<<<< add other sensor of T+RH+hPa types here
		default:
			return _dataPoint;
	}

	//Apply offsets
	_dataPoint.Temperature.floatVal += 0;
	_dataPoint.Humidity.floatVal += 0;
	
	// Calculate derived sensor values
	_dataPoint.DewPoint.floatVal = pow((_dataPoint.Humidity.floatVal / 100.0F), 0.125F) * (112 + (0.9F * _dataPoint.Temperature.floatVal)) + (0.1F * _dataPoint.Temperature.floatVal) - 112;
	_dataPoint.Altitude.floatVal = 44330.0 * (1.0 - pow(_dataPoint.Pressure.floatVal / SEALEVELPRESSURE_HPA, 0.1903));

	//Create string values from the float Data Point Values

	_dataPoint.Temperature.strVal = String(_dataPoint.Temperature.floatVal, 1);
	_dataPoint.DewPoint.strVal = String(_dataPoint.DewPoint.floatVal, 1);
	
	// Scale F values
	_dataPoint.Temperature_F.floatVal = (_dataPoint.Temperature.floatVal * 9 / 5) + 32;
	_dataPoint.DewPoint_F.floatVal = (_dataPoint.DewPoint.floatVal * 9 / 5) + 32;
	_dataPoint.Temperature_F.strVal = String((_dataPoint.Temperature.floatVal * 9 / 5) + 32, 1);
	_dataPoint.DewPoint_F.strVal = String((_dataPoint.DewPoint.floatVal * 9 / 5) + 32, 1);

	_dataPoint.Humidity.strVal = String(_dataPoint.Humidity.floatVal, 1);

	// Pressure scales
	_dataPoint.Pressure.strVal = String(_dataPoint.Pressure.floatVal, 1);
	_dataPoint.Pressure_Bar.floatVal = (float)_dataPoint.Pressure.floatVal / (float)1000;
	_dataPoint.Pressure_Bar.strVal = String((float)_dataPoint.Pressure.floatVal / (float)1000, 3);

	_dataPoint.Altitude.strVal = String(_dataPoint.Altitude.floatVal, 1);

	return _dataPoint;
}



// Main Sensor Data processing function
void MySensorClass::logData() {	
	// Read sensors
	dataPoint = readData();
	// Updated LCD buffer
	loadLCD();
}



//Load LCD's buffer with date from the sensors
void MySensorClass::loadLCD() {
	//Data structure coversions into formated strings for display
	String LCD_Buffer[MAX_BUFF];
	LCD_Buffer[0] = "";

	//LCD_Buffer[1] = dataPoint.Temperature_F.strVal + "*F\n";	
	LCD_Buffer[1] = dataPoint.Temperature.strVal + "*C\n";

	LCD_Buffer[2] = dataPoint.Humidity.strVal + "%";
	
	//LCD_Buffer[3] = dataPoint.Pressure_Bar.strVal + "bar";	
	LCD_Buffer[3] = dataPoint.Pressure.strVal + "hPa";
	

	//	LCD_Buffer[4] = dataPoint.DewPoint_F.strVal + "*F\n";
	LCD_Buffer[4] = dataPoint.DewPoint.strVal + "*C\n";

	LCD.loadBuffer(LCD_Buffer);
}
