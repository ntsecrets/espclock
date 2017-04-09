/*
 * Free to use software from Ewald Kendziorra, 2015 - www.kendziorra.nl
 * ### ESP 8266 NTP Time Server lib - use the demo ###
 */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "NtpTime.h"
#include <TimeLib.h>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

NTP ntp = NTP(); 
WiFiUDP Udp;

void NTP::syncTheTime() {
  ntp.timeIsSynced = false;
  IPAddress timeServerIP; // NTP server address
  WiFi.hostByName(ntp.ntpServerName, timeServerIP);  // get address from pool
  Udp.begin(ntp.localPort);
  byte pcktBuf[48];
  memset(pcktBuf, 0, 48);
  pcktBuf[0] = 0b11100011;
  pcktBuf[2] = 6;
  pcktBuf[3] = 0xEC;
  pcktBuf[12] = 49;
  pcktBuf[13] = 0x4E;
  pcktBuf[14] = 49;
  pcktBuf[15] = 52;
  Udp.beginPacket(timeServerIP, 123);  // requests to port 123
  Udp.write(pcktBuf, 48);  // send the packed
  Udp.endPacket();
  uint32_t timeout = millis() + 2000;
  while (millis() < timeout) {
    if (Udp.parsePacket() >= 48) {  // get udp packet
      Udp.read(pcktBuf, 48);  // get the time data
      Udp.stop();
      // convert four bytes starting at location 40 to a long integer
      ntp.timestamp =  (unsigned long)pcktBuf[40] << 24;
      ntp.timestamp |= (unsigned long)pcktBuf[41] << 16;
      ntp.timestamp |= (unsigned long)pcktBuf[42] << 8;
      ntp.timestamp |= (unsigned long)pcktBuf[43];
      ntp.timestamp -= 2208988800UL;  // UTC/GMT timeSerial << "@" << endl;
      // convert four bytes starting at location 44 to a long integer
      ntp.milliseconds =  (unsigned long)pcktBuf[44] << 24;
      ntp.milliseconds |= (unsigned long)pcktBuf[45] << 16;
      ntp.milliseconds |= (unsigned long)pcktBuf[46] << 8;
      ntp.milliseconds |= (unsigned long)pcktBuf[47];
      ntp.milliseconds = (unsigned int)(ntp.milliseconds * 2.3283064365387E-07);
      ntp.startMillis = millis();
      ntp.syncTime = ntp.timestamp + ntp.syncOffset;
      ntp.timeIsSynced = true;
      lastSync = ntp.timestamp;


      // leap indicator stuff
      /* checking just the bits of packet 0 although I bet there is a better way to do this
       *    Leap Indicator (2 bits)
          This field indicates whether the last minute of the current day is to have a leap second applied. The field values follow:
          0: No leap second adjustment
          1: Last minute of the day has 61 seconds
          2: Last minute of the day has 59 seconds
          3: Clock is unsynchronized
       */
      if (CHECK_BIT(pcktBuf[0],7)) {
         ntp.LI = 2;
         
      }
      if (CHECK_BIT(pcktBuf[0],6)) {
         ntp.LI = 1;
         
      }

      if (CHECK_BIT(pcktBuf[0],6) && CHECK_BIT(pcktBuf[0],7)) {
         ntp.LI = 3;
         
      }

       if (!CHECK_BIT(pcktBuf[0],6) && !CHECK_BIT(pcktBuf[0],7)) {
         ntp.LI = 0;
         
      }
      
      break;
    }
  }
}

void NTP::getTimestamp() { // timestamp in millis (UNIX * 1000)
  char m[4];
  ultoa(ntp.timestamp, msTimestamp, 10);
  ultoa(ntp.milliseconds, m, 10);
  strcat(ntp.msTimestamp, "000");
  strcpy(ntp.msTimestamp + 13 - strlen(m), m);  
}

void NTP::addTime() {
  if (ntp.timestamp >= ntp.syncTime) ntp.syncTheTime();
  ntp._stop = millis();
  ntp._start = ntp.startMillis;
  ntp.startMillis = millis();
  if(ntp._stop < ntp._start)  // overflow=true
    ntp.difference = (unsigned long)4294967295 - ntp._start + ntp._stop + 1;
  else                // overflow=false
    ntp.difference = ntp._stop - ntp._start; 
  ntp.difference += ntp.milliseconds; 
  ntp.milliseconds = ntp.difference % 1000;
  ntp.timestamp += ntp.difference / 1000;
  NTP::getTimestamp(); 
}

const char *NTP::getTimeDate(time_t tm) {
  char *dt = new char[20];
  char buf[20];

  snprintf(buf, sizeof(buf), "%d-%02d-%02dT%02d:%02d:%02d", year(tm), month(tm),
           day(tm), hour(tm), minute(tm), second(tm));
  strcpy(dt, buf);

  return dt;
}

