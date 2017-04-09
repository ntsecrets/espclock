#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Timezone.h>
#include <EEPROM.h>
//#include <time.h>
#include <TimeLib.h>

Adafruit_7segment matrix = Adafruit_7segment();
Adafruit_7segment matrix2 = Adafruit_7segment();

const uint16_t
dash = 0b0000000001000000,
letter_a = 0b0000000011110111, // A
letter_p = 0b0000000011110011; // P

// Remember if the colon was drawn on the display so it can be blinked
// on and off every second.
bool blinkColon = false;


uint16_t milliseconds;
long unsigned int timeStamp = 0;
int lastsecond = 0;
bool leapApplied = false;
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

  clearDigits();

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


// IP Display handler.

void displayIP(bool half) {
  // we want to just blow through this without a delay, so it doesn't hang up stuff
  if (!clockMode) {
    if (!half) {
      matrix.print(WiFi.softAPIP()[0], DEC);
      matrix2.print(WiFi.softAPIP()[1], DEC);
    } else {
      matrix.print(WiFi.softAPIP()[2], DEC);
      matrix2.print(WiFi.softAPIP()[3], DEC);
    }
  } else {
    if (!half) {
      matrix.print(WiFi.localIP()[0], DEC);
      matrix2.print(WiFi.localIP()[1], DEC);
    } else {
      matrix.print(WiFi.localIP()[2], DEC);
      matrix2.print(WiFi.localIP()[3], DEC);
    }
  }
  display();
}



// end Ip display handler.

void displayClock() {
  //might need this for updating without restart
  int h = 0;
  int m = 0;
  int s = 0;


  //local = myTZ.toLocal(NTP.getTime(), &tcr);

  local = myTZ.toLocal(ntp.timestamp + settings.fudge, &tcr);

//ntp.LI = 2;

  //leap second?  Work in progress!
  if (ntp.LI != 0 && ntp.LI != 3) {

    if (hour(ntp.timestamp + settings.fudge) == 0 && minute(ntp.timestamp + settings.fudge) == 0 && second(ntp.timestamp + settings.fudge) == 0) {  //is it midnight UTC?

      if (ntp.LI == 1) {
        h = hour(local - 1);
        m = minute(local - 1);
        s = 60;
      } else {  // skip over 59 ok so this doesn't actually skip over 59 but it does remove the second at midnight - going back is less likely
        h = hour(local + 1);
        m = minute(local + 1);
        s = 0;
      }

      leapApplied = true;

    } else {
      if (leapApplied) {
        // sub 1 second to time
       if (ntp.LI == 1) {
        h = hour(local - 1);
        m = minute(local - 1);
        s = second(local - 1);
      } else {  // skip over 59
        h = hour(local + 1);
        m = minute(local + 1);
        s = second(local + 1);
      }

      } else {  // day leading up to leap second (normal time)
        h = hour(local);
        m = minute(local);
        s = second(local);
      }


    }

  } else {   //normal operation
    leapApplied = false;

    h = hour(local);
    m = minute(local);
    s = second(local);

  }

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

  // Do 24 hour to 12 hour format conversion when required.
  if (settings.twelvehr != 0) {
    // Handle when hours are past 12 by subtracting 12 hours (1200 value).
    if (h > 12) {
      displayValue -= 1200;
    }
    // Handle hour 0 (midnight) being shown as 12.
    else if (h == 0) {
      displayValue += 1200;
    }
  }

  if (s % 2 == 0) {
    blinkColon = true;
  } else {
    blinkColon = false;
  }

  boolean drawDots = false;
  if (!ntp.timeIsSynced && blinkColon == true) {
    displayDash();
  } else {

    //milliseconds = (millis() - timeStamp) / 10;




    matrix2.print(s * 100 + milliseconds, DEC);

    matrix2.drawColon(blinkColon);  //this has to go after or else it doesn't work

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




    //  handle milliseconds


    milliseconds = (millis() - timeStamp) / 10;



    if (milliseconds > 99 || lastsecond < s) {
      milliseconds = 0;
      timeStamp = millis();

    }





    lastsecond = s;



    if (milliseconds == 0) { //only update if it needs it (on the second)

      if (firstSync == 0 && ntp.lastSync != 0) {
        firstSync = ntp.lastSync;
      }


      matrix.print(displayValue, DEC);
      if (ntp.lastSync + 1 == ntp.timestamp) {
        matrix.writeDigitNum(4, displayValue % 10  , true);
        // synced = false;
      }

      matrix.drawColon(blinkColon);

      // set this back to false, so its on for 1 second (I hope)

      //0134
      // pad the minutes
      if (m < 10) {
        matrix.writeDigitNum(3, 0);
      }
      // need 00 for when its 0
      if (h == 0 && settings.twelvehr == 0) {  //only do this when in 24 hour mode
        matrix.writeDigitNum(1, 0);
      }

    }

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


void display8s() {
  matrix.print(8888, DEC);
  matrix2.print(8888, DEC);
  matrix.drawColon(true);
  matrix2.drawColon(true);
  display();

}


void displayVersion() {
  clearDigits();
  matrix.writeDigitNum(4, VERSION_MAJOR, true);
  matrix2.print( VERSION_MINOR, DEC);
  display();

}


