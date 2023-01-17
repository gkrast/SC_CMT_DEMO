#include "myLCD.h"

myLCD LCD;

//Create the display object
SSD1306Wire display(0x3c, SDA, SCL);

/// <summary>
/// Initializations of the LCD library
/// </summary>
void myLCD::Init()
{
    display.init();  
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
    font = ArialMT_Plain_10;
}


/// <summary>
/// Change the current font size
/// </summary>
/// <param name="f"></param>
void myLCD::fontSize(font_sizes f) {
    switch (f) {
        case SMALL:           
            font = Arial_Small;
            break;
        case MEDIUM:
            font = Arial_Medium;
            break;
        case LARGE:
            font = Arial_Large;
            break;
    }
    display.setFont(font);
}

/// <summary>
/// Update LCD with sensor data
/// </summary>
void myLCD::showData() {
    
    // LCD information holding time
    if (millis() < refreshTimer) return;
    refreshTimer = millis()+holdTime;

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    fontSize(MEDIUM);
    index++;   
    if (index > 4) {
        index = 1;
    }  
    switch (index) {
    case 1:
        display.drawString(0, 0, "Temperature");
        break;
    case 2:
        display.drawString(0, 0, "Humidity");
        break;
    case 3:
        display.drawString(0, 0, "Pressure");
        break;
    case 4:
        display.drawString(0, 0, "Dew Point");
        break;
    }
    //draw horizontal line
    display.drawHorizontalLine(0, 22, 128);
    
    //sensor data
    fontSize(LARGE);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 28, buffer[index]);
    
    //data and time
    fontSize(SMALL);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 54, buffer[0]);
    
    display.display();
}

/// <summary>
/// Shows running messages on the LCD with title or without
/// </summary>
void myLCD::showText(String text) {
    int k = 0;
    if (title == "") {
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        fontSize(SMALL);
        display.clear();
        //Move up one position when more than 5 lines
        if (index > 4) {
            for (k = 0; k < 4; k++) {
                buffer[k] = buffer[k + 1];
            }
            buffer[4] = text;
        }
        else {
            buffer[index] = text;
            index++;
        }
        for (k = 0; k < 5; k++) {
            display.drawString(0, (k * 12), buffer[k]);
        }
    }else{
        //draw title
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        fontSize(MEDIUM);
        display.drawString(0, 0, title);
        //draw horizontal line
        display.drawHorizontalLine(0, 20, 128);
        //push messages under the line
        fontSize(SMALL);
        //Move up one position when more than 3 lines
        if (index > 2) {
            for (k = 0; k < 2; k++) {
                buffer[k] = buffer[k + 1];
            }
            buffer[2] = text;
        }
        else {
            buffer[index] = text;
            index++;
        }
        for (k = 0; k < 3; k++) {
            display.drawString(0, 23 + (k * 12), buffer[k]);
        }
    }
    display.display();
    delay(holdTime);
}

/// <summary>
/// loads the internal message buffer with data
/// </summary>
/// <param name="text"></param>
void myLCD::loadBuffer(String text[MAX_BUFF]) {
    int k = 0;
    for (k = 0; k < MAX_BUFF; k++) {
        buffer[k] = text[k];
    }
}

/// <summary>
/// Sets the index of the LCD queue to zero
/// </summary>
void myLCD::clearLCD() {
    int k;
    for (k = 0; k < MAX_BUFF; k++) {
        buffer[k] = "";
    }
    display.clear();
    index = 0;
    title = "";
}

/// <summary>
/// Set title of a view with Title
/// </summary>
/// <param name="text"></param>
void myLCD::setTitle(String text) {
    title=text;
}

/// <summary>
/// Sets the time to hold display before 
/// refreshing with new data 
/// </summary>
/// <param name="sec"></param>
void myLCD::setHoldTime(int sec) {
    holdTime = sec;
}

/// <summary>
/// Shows Name and Revsion of the App
/// </summary>
void myLCD::appRev() {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    fontSize(LARGE);
    display.drawString(64, 12, APP);
    fontSize(SMALL);
    display.drawString(64, 42, "Version "+ (String)REV);
    display.display();
    delay(3000);
}


