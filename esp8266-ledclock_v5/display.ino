#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"



#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <EEPROM.h>



Adafruit_7segment matrix = Adafruit_7segment();
Adafruit_7segment matrix2 = Adafruit_7segment();

const uint16_t
dash = 0b0000000001000000,
letter_a = 0b0000000011110111, // A
letter_p = 0b0000000011110011; // P

// Remember if the colon was drawn on the display so it can be blinked
// on and off every second.
bool blinkColon = false;


int milliseconds;
long unsigned int timeStamp = 0;
int lastsecond = 0;
time_t local;

//set up the timezone stuff
//TimeChangeRule DT = { "DT", settings.dstWeek, settings.dstDayofweek, settings.dstMonth, settings.dstHour, settings.dstOffset};  //UTC - 4 hours
//TimeChangeRule ST = { "ST", settings.stdWeek, settings.stdDayofweek, settings.stdMonth, settings.stdHour, settings.stdOffset};   //UTC - 5 hours

//US Eastern Time Zone (New York, Detroit)
//TimeChangeRule DT = {"EDT", Second, Sun, Mar, 2, -240};    //Daylight time = UTC - 4 hours
//TimeChangeRule ST = {"EST", First, Sun, Nov, 2, -300};     //Standard time = UTC - 5 hours
//Timezone myTZ(settings.DT, settings.ST);

//  TimeChangeRule DT = { "DT", settings.dstWeek, settings.dstDayofweek, settings.dstMonth, settings.dstHour, settings.dstOffset};  //UTC - 4 hours
//  TimeChangeRule ST = { "ST", settings.stdWeek, settings.stdDayofweek, settings.stdMonth, settings.stdHour, settings.stdOffset};   //UTC - 5 hours
//   Timezone myTZ(DT, ST);



//Timezone myTZ(600);

TimeChangeRule DT = settings.LoadDT();
TimeChangeRule ST = settings.LoadST();

TimeChangeRule *tcr;
Timezone myTZ( DT, ST);





//char digits[4];
//char decimals;

//#define PULSE digitalWrite(CLOCK, LOW); delayMicroseconds(10) ; digitalWrite(CLOCK, HIGH); delayMicroseconds(10) ;

void clear() {
  matrix.clear();
  matrix2.clear();
}

void display() {




  matrix.writeDisplay();
  matrix2.writeDisplay();
}

void displayAP() {
  /* digits[0] = 0x10;
    digits[1] = 0xA;
    digits[2] = 0x18;
    digits[3] = 0x18;  */


  matrix.writeDigitRaw(1, letter_a);
  matrix.writeDigitRaw(3, letter_p);

  display();
  delay(1000);
}

void displayDash() {
  matrix.writeDigitRaw(0, dash);
  matrix.writeDigitRaw(1, dash);
  matrix.writeDigitRaw(3, dash);
  matrix.writeDigitRaw(4, dash);

  matrix2.writeDigitRaw(0, dash);
  matrix2.writeDigitRaw(1, dash);
  matrix2.writeDigitRaw(3, dash);
  matrix2.writeDigitRaw(4, dash);
  display();
}

void clearDigits() {

  matrix.clear();
  matrix2.clear();
}
/*
  // Twirler handler.
  Ticker ticker;

  volatile char busySegment = 0x12;
  volatile char busyDigit;  */

void displayBusy(char digit) {
  //  busyDigit = digit;
  // ticker.attach(0.1, _displayBusy);

  displayDash();
  //  display();
}

void stopDisplayBusy() {
  clearDigits();
}


// End twirler handler.

// IP Display handler.
//volatile signed char dispOctet = -1;

char displayIP() {

  if (!clockMode) {
    matrix.print(WiFi.softAPIP()[0], DEC);
    matrix.writeDisplay();

    matrix2.print(WiFi.softAPIP()[1], DEC);
    matrix2.writeDisplay();
    delay(1000);
    matrix.print(WiFi.softAPIP()[2], DEC);
    matrix.writeDisplay();

    matrix2.print(WiFi.softAPIP()[3], DEC);
    matrix2.writeDisplay();
    //   delay(1000);

  } else {

    matrix.print(WiFi.localIP()[0], DEC);
    matrix.writeDisplay();

    matrix2.print(WiFi.localIP()[1], DEC);
    matrix2.writeDisplay();
    delay(1000);
    matrix.print(WiFi.localIP()[2], DEC);
    matrix.writeDisplay();

    matrix2.print(WiFi.localIP()[3], DEC);
    matrix2.writeDisplay();
    delay(1000);
  }
  //  display();
  return false;
}



// end Ip display handler.

void displayClock() {
  //might need this for updating without restart



 // local = myTZ.toLocal(now(), &tcr);

 local = myTZ.toLocal(now() + settings.fudge, &tcr);


  /*  int h = hour();
    int m = minute();
    int s = second();    */

  int h = hour(local);
  int m = minute(local);
  int s = second(local);


  int displayValue;

  // dim the display if its in the hours

  if (settings.dim == 0 || settings.bright == 0) {
    matrix.setBrightness(15);
    matrix2.setBrightness(15);
  } else {

    if (h >= settings.dim || h <= settings.bright) {
      matrix.setBrightness(1);
      matrix2.setBrightness(1);
    } else {
      matrix.setBrightness(15);
      matrix2.setBrightness(15);
    }
  }




  // Show the time on the display by turning it into a numeric
  // value, like 3:30 turns into 330, by multiplying the hour by
  // 100 and then adding the minutes.
  displayValue = h * 100 + m;
  /*byte twentyfourhr = 0x01;
  // Do 24 hour to 12 hour format conversion when required.
  if (twentyfourhr != 0x01) {
    // Handle when hours are past 12 by subtracting 12 hours (1200 value).
    if (h > 12) {
      displayValue -= 1200;
    }
    // Handle hour 0 (midnight) being shown as 12.
    else if (h == 0) {
      displayValue += 1200;
    }
  }  */

  if (s % 2 == 0) {
    blinkColon = true;
  } else {
    blinkColon = false;
  }

  boolean drawDots = false;
  if (timeStatus() != timeSet && blinkColon == false) {
    displayDash();
  } else {

 




    matrix2.print(s * 100 + milliseconds, DEC);

    matrix2.drawColon(blinkColon);  //this has to go after or else it doesn't work

  }

  // matrix2.print( milliseconds, DEC);


  //  handle milliseconds


  milliseconds = (millis() - timeStamp) / 10;

  if (milliseconds > 99 || lastsecond < s) {
    milliseconds = 0;
    timeStamp = millis();

  }

  // pad the seconds
  if (s < 10) {
    matrix2.writeDigitNum(0, 0);
  }
  // need 00 for when its 0
  if (s == 0) {
    matrix2.writeDigitNum(1, 0);
  }

  if (s == 0 && milliseconds == 0) {
    matrix2.writeDigitNum(3, 0);
  }

  lastsecond = s;



 if (milliseconds == 0){  //only update if it needs it (on the second)
    matrix.print(displayValue, DEC);

    matrix.drawColon(blinkColon);
 }


  matrix.writeDisplay();
 
  
  matrix2.writeDisplay();


  //display();
}

void setupDisplay() {
  Wire.begin(0, 2); // Define pins for SDA and SCL.
  matrix.begin(0x70);
  matrix2.begin(0x71);
  //displayDash();
  // displayID();


}


void displayID() {
uint16_t ua, ub;
ua = (uint16_t) (ESP.getChipId() >> 16);
ub = (uint16_t) (ESP.getChipId() & 0x0000FFFFuL);

matrix.print(ua, HEX);
matrix2.print(ub, HEX);
display();

}

