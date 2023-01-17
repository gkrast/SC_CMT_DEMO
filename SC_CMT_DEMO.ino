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

	This code demonstrates the functionality of the CMT-B sensor board
	----------------------------------------------------------------------------------------------------------
	Application description:
	----------------------------------------------------------------------------------------------------------
	During start up the program shows initialization  messages on the display
	The sensor circuits HCP2080 (Temperature and Humidity) and MS560702BA03-50 (Pressure) are then sampled continuously.
	and the reading are shown on the display.
	This application code is provided as is without any warrantee. Its sole purpose is to help in the development of applications
	using the selected digital sensors and NodeMCU Wi-Fi module.
*/

// LCD driver libraries
#include <SSD1306Wire.h>
#include <OLEDDisplayUi.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplay.h>

// Driver for IIC communication
#include <Wire.h>

#include "app.h"				// Application classes


void setup() {
	Serial.begin(9600);
	cout << "Initilizing..." << endl << endl;

	LCD.Init();
	LCD.appRev();
	LCD.setTitle("Initializing...");
	LCD.setHoldTime(1000);

	Sensors.init();

	LCD.clearLCD();
	LCD.setHoldTime(3000); // ENTER LCD ROTATIONAL DISPLAY MODE
}

void loop() {

	// Process the sensors
	if (Sensors.sensorsConnected) {
		Sensors.logData();
	}
	LCD.showData();

}
