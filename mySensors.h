// mySensors.h

#ifndef _MYSENSORS_h
#define _MYSENSORS_h

#include "app.h"

class MySensorClass {

public:

	bool sensorsConnected = false;		//Sensors connection status
	dataPointVals dataPoint;			//Last sensor data readings
	dataStruct logDataVals;				//Last sensor data readings ready for recording in the log file

	int logInterval = 30;				//Logging Interval in seconds
	bool inAlarm = false;				//Current Alarm status
	String strAlarmMsg;					//Current Alarm message formated for the LCD
	SENSOR_TYPES sensorTypes;			//Sensor type identifier
	
	bool init();						//Initialize the current sensors
	void logData();						//Data logging at interval
	dataPointVals readData();			//Read data for real time display
	

private:
	void checkAlarms();					//Checks last data poin for alarm condition
	void loadLCD();						//Load the LCD buffer with last sensor readings for display	
	
	
};

extern MySensorClass Sensors;

#endif

