


#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>


#include <EEPROM.h>


#ifndef SETTINGS_H
#define SETTINGS_H

#define EEPROM_WIFI_SIZE 512
#define EEPROM_MAGIC "NtPc"
#define EEPROM_MAGIC_OFFSET 0
#define EEPROM_MAGIC_LENGTH 4
#define EEPROM_SSID_OFFSET EEPROM_MAGIC_OFFSET + EEPROM_MAGIC_LENGTH
#define EEPROM_SSID_LENGTH 32
#define EEPROM_PSK_OFFSET EEPROM_SSID_OFFSET + EEPROM_SSID_LENGTH
#define EEPROM_PSK_LENGTH 64
#define EEPROM_TZ_OFFSET EEPROM_PSK_OFFSET + EEPROM_PSK_LENGTH
#define EEPROM_TIMESERVER_OFFSET EEPROM_TZ_OFFSET + 1
#define EEPROM_TIMESERVER_LENGTH 32
#define EEPROM_INTERVAL_OFFSET EEPROM_TIMESERVER_OFFSET + EEPROM_TIMESERVER_LENGTH
#define EEPROM_INTERVAL_LENGTH 2
#define EEPROM_NAME_OFFSET EEPROM_INTERVAL_OFFSET + EEPROM_INTERVAL_LENGTH
#define EEPROM_NAME_LENGTH 32

// my own stuff

//US Eastern Time Zone (New York, Detroit)
//TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240};    //Daylight time = UTC - 4 hours
//TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};     //Standard time = UTC - 5 hours
//Timezone myTZ(myDST, mySTD);

#define EEPROM_DT_OFFSET EEPROM_NAME_OFFSET + EEPROM_NAME_LENGTH
#define EEPROM_DT_LENGTH 7

#define EEPROM_DT_WEEK_OFFSET EEPROM_DT_OFFSET + EEPROM_DT_LENGTH
#define EEPROM_DT_WEEK_LENGTH 1

#define EEPROM_DT_DAYOFWEEK_OFFSET EEPROM_DT_WEEK_OFFSET + EEPROM_DT_WEEK_LENGTH
#define EEPROM_DT_DAYOFWEEK_LENGTH 1

#define EEPROM_DT_MONTH_OFFSET EEPROM_DT_DAYOFWEEK_OFFSET + EEPROM_DT_DAYOFWEEK_LENGTH
#define EEPROM_DT_MONTH_LENGTH 1

#define EEPROM_DT_HOUR_OFFSET EEPROM_DT_MONTH_OFFSET + EEPROM_DT_MONTH_LENGTH
#define EEPROM_DT_HOUR_LENGTH 1

#define EEPROM_DT_OFFSET_OFFSET EEPROM_DT_HOUR_OFFSET + EEPROM_DT_HOUR_LENGTH
#define EEPROM_DT_OFFSET_LENGTH 4

// standard

#define EEPROM_ST_OFFSET EEPROM_DT_OFFSET_OFFSET + EEPROM_DT_OFFSET_LENGTH
#define EEPROM_ST_LENGTH 7

#define EEPROM_ST_WEEK_OFFSET EEPROM_ST_OFFSET + EEPROM_ST_LENGTH
#define EEPROM_ST_WEEK_LENGTH 1

#define EEPROM_ST_DAYOFWEEK_OFFSET EEPROM_ST_WEEK_OFFSET + EEPROM_ST_WEEK_LENGTH
#define EEPROM_ST_DAYOFWEEK_LENGTH 1

#define EEPROM_ST_MONTH_OFFSET EEPROM_ST_DAYOFWEEK_OFFSET + EEPROM_ST_DAYOFWEEK_LENGTH
#define EEPROM_ST_MONTH_LENGTH 1

#define EEPROM_ST_HOUR_OFFSET EEPROM_ST_MONTH_OFFSET + EEPROM_ST_MONTH_LENGTH
#define EEPROM_ST_HOUR_LENGTH 1

#define EEPROM_ST_OFFSET_OFFSET EEPROM_ST_HOUR_OFFSET + EEPROM_ST_HOUR_LENGTH
#define EEPROM_ST_OFFSET_LENGTH 4

#define EEPROM_DIM_OFFSET EEPROM_ST_OFFSET_OFFSET + EEPROM_ST_OFFSET_LENGTH
#define EEPROM_DIM_OFFSET_LENGTH 1

#define EEPROM_BRIGHT_OFFSET EEPROM_DIM_OFFSET + EEPROM_DIM_OFFSET_LENGTH
#define EEPROM_BRIGHT_OFFSET_LENGTH 1

#define EEPROM_FUDGE_OFFSET EEPROM_BRIGHT_OFFSET + EEPROM_BRIGHT_OFFSET_LENGTH
#define EEPROM_FUDGE_OFFSET_LENGTH 4

//#define TZRULESIZLE 15  // 15 bytes each



#define DEFAULT_TIMESERVER "time.nist.gov"
#define MINIMUM_INTERVAL 60
#define DEFAULT_INTERVAL 3600

#define DEFAULT_DT "EDT  "
#define DEFAULT_ST "EST  "

//TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240};

#define DEFAULT_DT_WEEK 2
#define DEFAULT_DT_DAYOFWEEK 1
#define DEFAULT_DT_MONTH 3
#define DEFAULT_DT_HOUR 2
#define DEFAULT_DT_OFFSET -240

#define DEFAULT_ST_WEEK 1
#define DEFAULT_ST_DAYOFWEEK 1
#define DEFAULT_ST_MONTH 11
#define DEFAULT_ST_HOUR 2
#define DEFAULT_ST_OFFSET -300

#define DEFAULT_DIM 22
#define DEFAULT_BRIGHT 7

// TODO finish the defaults

#define CLOCK_NAME "ESP-CLOCK-"
#define WIFI_AP_NAME CLOCK_NAME


class Settings {

  public:
    Settings() {};

    void Load() {
      char buffer[EEPROM_WIFI_SIZE];
      EEPROM.begin(EEPROM_WIFI_SIZE);
      for (int i = 0 ; i < EEPROM_WIFI_SIZE ; i++) {
        buffer[i] = EEPROM.read(i);
      }
      EEPROM.end();

      // Verify magic;
      String magic;
      for (int i = EEPROM_MAGIC_OFFSET ; i < EEPROM_MAGIC_OFFSET + EEPROM_MAGIC_LENGTH ; i++) {
        magic += buffer[i];
      }
      if (magic != EEPROM_MAGIC) {
        // load DEFAULTS
        SaveDefaults();
      }
      // Read SSID
      ssid = "";
      for (int i = EEPROM_SSID_OFFSET ; i < EEPROM_SSID_OFFSET + EEPROM_SSID_LENGTH ; i++) {
        if (buffer[i]) ssid += buffer[i];
      }
      // Read PSK
      psk = "";
      for (int i = EEPROM_PSK_OFFSET ; i < EEPROM_PSK_OFFSET + EEPROM_PSK_LENGTH ; i++) {
        if (buffer[i]) psk += buffer[i];



      }

      /*
              char tzbufferDT[EEPROM_DT_OFFSET];
            EEPROM.begin(TZRULESIZLE);
            for (int i = 0 ; i < TZRULESIZLE ; i++) {
              tzbufferDT[i] = EEPROM.read(i);
            }
            EEPROM.end();

        //      TimeChangeRule DT = { *tzbuffer };

             char tzbufferST[EEPROM_DT_OFFSET];
            EEPROM.begin(TZRULESIZLE * 2);
            for (int i = 0 ; i < TZRULESIZLE ; i++) {
              tzbufferST[i] = EEPROM.read(i);
            }
            EEPROM.end();   */



      //  timezone = long(buffer[EEPROM_TZ_OFFSET]);

      strncpy(timeserver, &buffer[EEPROM_TIMESERVER_OFFSET], EEPROM_TIMESERVER_LENGTH);
      if (strlen(timeserver) < 1) {
        strcpy(timeserver, DEFAULT_TIMESERVER);
      }

      // 1 DST CODE
      strncpy(DST, &buffer[EEPROM_DT_OFFSET], EEPROM_DT_LENGTH);
      if (strlen(DST) < 1) {
        strcpy(DST, DEFAULT_DT);
      }

      //2 dST day of week
      dstWeek = uint8_t(buffer[EEPROM_DT_WEEK_OFFSET]);
      //  if (dstWeek == 0){
      //   dstWeek = DEFAULT_DT_WEEK;
      // }
      // day of week
      dstDayofweek = uint8_t(buffer[EEPROM_DT_DAYOFWEEK_OFFSET]);
      if (dstDayofweek == 0) {
        dstDayofweek = DEFAULT_DT_DAYOFWEEK;
      }
      //4 dstMonth
      dstMonth = uint8_t(buffer[EEPROM_DT_MONTH_OFFSET]);
      if (dstMonth == 0) {
        dstMonth = DEFAULT_DT_MONTH;
      }
      //5 dstHour
      dstHour = uint8_t(buffer[EEPROM_DT_HOUR_OFFSET]);

      //6 dst Offset
      dstOffset = ( ((unsigned long)buffer[EEPROM_DT_OFFSET_OFFSET] << 24)
                    + ((unsigned long)buffer[EEPROM_DT_OFFSET_OFFSET + 1] << 16)
                    + ((unsigned long)buffer[EEPROM_DT_OFFSET_OFFSET + 2] << 8)
                    + ((unsigned long)buffer[EEPROM_DT_OFFSET_OFFSET + 3] ) );
      // dstOffset = int(buffer[EEPROM_DT_OFFSET_OFFSET]);

      // 1 STD Code
      strncpy(STD, &buffer[EEPROM_ST_OFFSET], EEPROM_ST_LENGTH);
      if (strlen(STD) < 1) {
        strcpy(STD, DEFAULT_ST);
      }

      //2 std day of week
      stdWeek = uint8_t(buffer[EEPROM_ST_WEEK_OFFSET]);
      // if (stdWeek == 0){
      //   stdWeek = DEFAULT_ST_WEEK;
      // }
      // day of week
      stdDayofweek = uint8_t(buffer[EEPROM_ST_DAYOFWEEK_OFFSET]);
      if (stdDayofweek == 0) {
        stdDayofweek = DEFAULT_ST_DAYOFWEEK;
      }
      //4 stdMonth
      stdMonth = uint8_t(buffer[EEPROM_ST_MONTH_OFFSET]);
      if (stdMonth == 0) {
        stdMonth = DEFAULT_ST_MONTH;
      }
      //5 stdHour
      stdHour = uint8_t(buffer[EEPROM_ST_HOUR_OFFSET]);

      //6 std Offset
      //stdOffset = long(buffer[EEPROM_ST_OFFSET_OFFSET]);  //it could be 0 so I can't default it... oh well. maybe handle it on the form?

      stdOffset = ( ((unsigned long)buffer[EEPROM_ST_OFFSET_OFFSET] << 24)
                    + ((unsigned long)buffer[EEPROM_ST_OFFSET_OFFSET + 1] << 16)
                    + ((unsigned long)buffer[EEPROM_ST_OFFSET_OFFSET + 2] << 8)
                    + ((unsigned long)buffer[EEPROM_ST_OFFSET_OFFSET + 3] ) );


      // dim / bright
      dim = int(buffer[EEPROM_DIM_OFFSET]);
      bright = int(buffer[EEPROM_BRIGHT_OFFSET]);

      fudge = int(buffer[EEPROM_FUDGE_OFFSET]);

      interval = time_t(buffer[EEPROM_INTERVAL_OFFSET]) << 8;
      interval |= buffer[EEPROM_INTERVAL_OFFSET + 1];
      if (interval < MINIMUM_INTERVAL) {
        interval = MINIMUM_INTERVAL;
      }
      // Clock name.
      name = "";
      for (int i = EEPROM_NAME_OFFSET ; i < EEPROM_NAME_OFFSET + EEPROM_NAME_LENGTH ; i++) {
        if (buffer[i]) name += buffer[i];
      }


      // TimeChangeRule DT = { "DT", dstWeek, dstDayofweek, dstMonth, dstHour, dstOffset};  //UTC - 4 hours
      // TimeChangeRule ST = { "ST", stdWeek, stdDayofweek, stdMonth, stdHour, stdOffset};   //UTC - 5 hours



    }

    void Save() {
      unsigned char buffer[EEPROM_WIFI_SIZE];
      memset(buffer, 0, EEPROM_WIFI_SIZE);

      // Copy magic to buffer;
      strncpy((char *)buffer, EEPROM_MAGIC, EEPROM_MAGIC_LENGTH);

      // Copy SSID to buffer;
      ssid.getBytes(&buffer[EEPROM_SSID_OFFSET], EEPROM_SSID_LENGTH, 0);
      // Copy PSK to buffer.
      psk.getBytes(&buffer[EEPROM_PSK_OFFSET], EEPROM_PSK_LENGTH, 0);
      // Copy timezone.
      // buffer[EEPROM_TZ_OFFSET] = timezone;
      // Copy timeserver.
      strncpy((char *)&buffer[EEPROM_TIMESERVER_OFFSET], (char *)timeserver, EEPROM_TIMESERVER_LENGTH);
      // Copy interval.
      buffer[EEPROM_INTERVAL_OFFSET] = interval  >> 8;
      buffer[EEPROM_INTERVAL_OFFSET + 1] = interval & 0xff;
      // Copy clock name.
      name.getBytes(&buffer[EEPROM_NAME_OFFSET], EEPROM_NAME_LENGTH, 0);

      //my timezone stuff
      //TZ name
      strncpy((char *)&buffer[EEPROM_DT_OFFSET], (char *)DST, EEPROM_DT_LENGTH);
      buffer[EEPROM_DT_WEEK_OFFSET] = dstWeek;
      buffer[EEPROM_DT_DAYOFWEEK_OFFSET] = dstDayofweek;
      buffer[EEPROM_DT_MONTH_OFFSET] = dstMonth;
      buffer[EEPROM_DT_HOUR_OFFSET] = dstHour;


      // convert from an unsigned long int to a 4-byte array
      buffer[EEPROM_DT_OFFSET_OFFSET] = (int)((dstOffset >> 24) & 0xFF) ;
      buffer[EEPROM_DT_OFFSET_OFFSET + 1] = (int)((dstOffset >> 16) & 0xFF) ;
      buffer[EEPROM_DT_OFFSET_OFFSET + 2] = (int)((dstOffset >> 8) & 0XFF);
      buffer[EEPROM_DT_OFFSET_OFFSET + 3] = (int)((dstOffset & 0XFF));;







      //    buffer[EEPROM_DT_OFFSET_OFFSET] = dstOffset;

      strncpy((char *)&buffer[EEPROM_ST_OFFSET], (char *)STD, EEPROM_ST_LENGTH);
      buffer[EEPROM_ST_WEEK_OFFSET] = stdWeek;
      buffer[EEPROM_ST_DAYOFWEEK_OFFSET] = stdDayofweek;
      buffer[EEPROM_ST_MONTH_OFFSET] = stdMonth;
      buffer[EEPROM_ST_HOUR_OFFSET] = stdHour;
      //buffer[EEPROM_ST_OFFSET_OFFSET] = stdOffset;


      // convert from an unsigned long int to a 4-byte array
      buffer[EEPROM_ST_OFFSET_OFFSET] = (int)((stdOffset >> 24) & 0xFF) ;
      buffer[EEPROM_ST_OFFSET_OFFSET + 1] = (int)((stdOffset >> 16) & 0xFF) ;
      buffer[EEPROM_ST_OFFSET_OFFSET + 2] = (int)((stdOffset >> 8) & 0XFF);
      buffer[EEPROM_ST_OFFSET_OFFSET + 3] = (int)((stdOffset & 0XFF));;



      buffer[EEPROM_DIM_OFFSET] = dim;
      buffer[EEPROM_BRIGHT_OFFSET] = bright;

      buffer[EEPROM_FUDGE_OFFSET] = fudge;



      // Write to EEPROM.
      EEPROM.begin(EEPROM_WIFI_SIZE);
      for (int i = 0 ; i < EEPROM_WIFI_SIZE ; i++) {
        EEPROM.write(i, buffer[i]);
      }
      EEPROM.end();


    }


    void SaveDefaults() {
      unsigned char buffer[EEPROM_WIFI_SIZE];
      memset(buffer, 0, EEPROM_WIFI_SIZE);

      // Copy magic to buffer;
      strncpy((char *)buffer, EEPROM_MAGIC, EEPROM_MAGIC_LENGTH);
      
      strncpy((char *)&buffer[EEPROM_TIMESERVER_OFFSET], DEFAULT_TIMESERVER, EEPROM_TIMESERVER_LENGTH);
      // Copy interval.
      interval = DEFAULT_INTERVAL;
      buffer[EEPROM_INTERVAL_OFFSET] = interval  >> 8;
      buffer[EEPROM_INTERVAL_OFFSET + 1] = interval & 0xff;
      // Copy clock name.
      name = CLOCK_NAME;
      name.getBytes(&buffer[EEPROM_NAME_OFFSET], EEPROM_NAME_LENGTH, 0);

      //my timezone stuff
      //TZ name
     // DST = DEFAULT_DT;
      
      strncpy((char *)&buffer[EEPROM_DT_OFFSET], (char *)DEFAULT_DT, EEPROM_DT_LENGTH);
      
      buffer[EEPROM_DT_WEEK_OFFSET] = DEFAULT_DT_WEEK;
      buffer[EEPROM_DT_DAYOFWEEK_OFFSET] = DEFAULT_DT_DAYOFWEEK ;
      buffer[EEPROM_DT_MONTH_OFFSET] = DEFAULT_DT_MONTH;
      buffer[EEPROM_DT_HOUR_OFFSET] = DEFAULT_DT_HOUR;

      dstOffset = DEFAULT_DT_OFFSET;
      // convert from an unsigned long int to a 4-byte array
      buffer[EEPROM_DT_OFFSET_OFFSET] = (int)((dstOffset >> 24) & 0xFF) ;
      buffer[EEPROM_DT_OFFSET_OFFSET + 1] = (int)((dstOffset >> 16) & 0xFF) ;
      buffer[EEPROM_DT_OFFSET_OFFSET + 2] = (int)((dstOffset >> 8) & 0XFF);
      buffer[EEPROM_DT_OFFSET_OFFSET + 3] = (int)((dstOffset & 0XFF));;







      //    buffer[EEPROM_DT_OFFSET_OFFSET] = dstOffset;
      //STD = DEFAULT_ST;
      strncpy((char *)&buffer[EEPROM_ST_OFFSET], (char *)DEFAULT_ST, EEPROM_ST_LENGTH);
      buffer[EEPROM_ST_WEEK_OFFSET] = DEFAULT_ST_WEEK;
      buffer[EEPROM_ST_DAYOFWEEK_OFFSET] = DEFAULT_ST_DAYOFWEEK;
      buffer[EEPROM_ST_MONTH_OFFSET] = DEFAULT_ST_MONTH;
      buffer[EEPROM_ST_HOUR_OFFSET] = DEFAULT_ST_HOUR;
      //buffer[EEPROM_ST_OFFSET_OFFSET] = stdOffset;

      stdOffset = DEFAULT_ST_OFFSET;
      // convert from an unsigned long int to a 4-byte array
      buffer[EEPROM_ST_OFFSET_OFFSET] = (int)((stdOffset >> 24) & 0xFF) ;
      buffer[EEPROM_ST_OFFSET_OFFSET + 1] = (int)((stdOffset >> 16) & 0xFF) ;
      buffer[EEPROM_ST_OFFSET_OFFSET + 2] = (int)((stdOffset >> 8) & 0XFF);
      buffer[EEPROM_ST_OFFSET_OFFSET + 3] = (int)((stdOffset & 0XFF));;



      buffer[EEPROM_DIM_OFFSET] =  DEFAULT_DIM;
      buffer[EEPROM_BRIGHT_OFFSET] = DEFAULT_BRIGHT;

      buffer[EEPROM_FUDGE_OFFSET] = 1;   /for now



      // Write to EEPROM.
      EEPROM.begin(EEPROM_WIFI_SIZE);
      for (int i = 0 ; i < EEPROM_WIFI_SIZE ; i++) {
        EEPROM.write(i, buffer[i]);
      }
      EEPROM.end();


    }

    TimeChangeRule LoadDT() {
      Load();
      // TimeChangeRule TZ = {"EDT", Second, Sun, Mar, 2, -240};

      //    TimeChangeRule DT = { "DST", dstWeek, dstDayofweek, dstMonth, dstHour, dstOffset};  //UTC - 4 hours

      TimeChangeRule DT; // = {"EDT", Second, Sun, Mar, 2, -240};  //UTC - 4 hours
      DT.dow = this->dstDayofweek;
      DT.week = this->dstWeek;
      DT.hour = this->dstHour;
      DT.offset = this->dstOffset;
      strncpy(DT.abbrev, DST, 6);
      DT.month = this->dstMonth;


      return DT;
    }

    TimeChangeRule LoadST() {
      //  Load();
      // TimeChangeRule TZ = {"EDT", Second, Sun, Mar, 2, -240};

      TimeChangeRule ST; // = {"EDT", Second, Sun, Mar, 2, -240};  //UTC - 4 hours
      ST.dow = this->stdDayofweek;
      ST.week = this->stdWeek;
      ST.hour = this->stdHour;
      ST.offset = this->stdOffset;
      strncpy(ST.abbrev, STD, 6);
      ST.month = this->stdMonth;


      return ST;
    }

    String ssid;
    String psk;
    //long timezone;
    char timeserver[64];
    int interval;
    String name;

    // dst  "EST", Second, Sun, Mar, 2, -240
    char DST[6];
    uint8_t dstWeek;
    uint8_t dstDayofweek;
    uint8_t dstMonth;
    uint8_t dstHour;
    int dstOffset;

    // std
    char STD[6];
    uint8_t stdWeek;
    uint8_t stdDayofweek;
    uint8_t stdMonth;
    uint8_t stdHour;
    int stdOffset;

    //  char tzbufferDT[15];
    //  char tzbufferST[15];

    int dim;
    int bright;

    int fudge;

    //  time_t local;

    //set up the timezone stuff
    //TimeChangeRule DT = { "DT", dstWeek, dstDayofweek, dstMonth, dstHour, dstOffset};  //UTC - 4 hours
    //TimeChangeRule ST = { "ST", stdWeek, stdDayofweek, stdMonth, stdHour, stdOffset};   //UTC - 5 hours

    //US Eastern Time Zone (New York, Detroit)
    //TimeChangeRule DT = {"EDT", Second, Sun, Mar, 2, -240};    //Daylight time = UTC - 4 hours
    //TimeChangeRule ST = {"EST", First, Sun, Nov, 2, -300};     //Standard time = UTC - 5 hours
    //Timezone myTZ(DT, ST);

    //TimeChangeRule *tcr;



};

Settings settings;

#endif




