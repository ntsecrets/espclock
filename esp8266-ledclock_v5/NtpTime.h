/*
 * Free to use software from Ewald Kendziorra, 2015 - www.kendziorra.nl
 * ### ESP 8266 NTP Time Server lib - use the demo ###
 */
#ifndef NTP_CLIENT
#define NTP_CLIENT

#include <Arduino.h>

class NTP{
public:
  char * ntpServerName;       // the NTP servers pool name
  char msTimestamp[14];       // timestamp in millis, 14 digits char string
  unsigned long startMillis;  // start millis for timer difference
  unsigned int localPort;     // (change it) local UDP port for Time Server
  unsigned long timestamp;    // UNIX timestamp (10 digits)
  unsigned int milliseconds;  // used for extended timestamp (3 digits)  
  unsigned int syncOffset;    // offset in sec to sync the time
  boolean timeIsSynced;       // is the clock synced?; test it before using the time after a reset
  uint8_t LI;                 // leap indicator!
  time_t lastSync;
  void addMilli();            // update ms
  void syncTheTime();         // sync timestamp and millis
  void addTime();
  const char* getTimeDate(time_t _time);
  IPAddress timeServerIP;     // for troubleshooting - get the last IP used
  uint32_t pktDelay;          // apparent delay in packet round trip
private:
  unsigned long difference;
  unsigned long _stop;
  unsigned long _start;
  unsigned long syncTime;
  void getTimestamp();
};

extern NTP ntp;

#endif


