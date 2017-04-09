const int VERSION_MAJOR = 5;
const int VERSION_MINOR = 61;

const char* www_username = "admin";
const char* updatePath = "/fwupload";

String ID = String(ESP.getChipId(), HEX);

char *www_password = new char[ID.length() + 1];


#include <Timezone.h>
#include "credits.h"

#include <EEPROM.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266WiFi.h>
#include <WiFiServer.h>

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "NtpTime.h"


#include <ESP8266HTTPUpdateServer.h>   //OTA update stuff!!
ESP8266HTTPUpdateServer httpUpdater;


//ESP8266HTTPUpdateServer httpUpdater2; //temporary backdoor


#include "settings.h"
#include "mainPage.h"

#define SETUP_PIN 3

//New feature, Set a password even though its hard coded - WPA2 will establish enough security to prevent spying
#define WPA_PSK "12345678"

#define MODE_SETUP 0
#define MODE_CLOCK 1
int clockMode;
//bool ipshown = false;
int wificonnects = 0;
uint8_t setupdisp = 0;
//bool synced = false;

time_t firstSync = 0;

ESP8266WebServer server (80);

String httpUpdateResponse;

//time_t prevDisplay = 0;

TimeChangeRule DT = settings.LoadDT();
TimeChangeRule ST = settings.LoadST();

TimeChangeRule *tcr;
Timezone myTZ( DT, ST);

void handleRoot() {

  if (!server.authenticate(www_username, String(ESP.getChipId(), HEX).c_str()) && clockMode == MODE_CLOCK) {
    return server.requestAuthentication();
  }

  String s = MAIN_page;
  s.replace("@@SSID@@", settings.ssid);
  //s.replace("@@PSK@@", settings.psk);
  //  s.replace("@@TZ@@", String(settings.timezone));
  s.replace("@@TIME@@", String(tcr -> abbrev) + " " + String(ntp.getTimeDate(myTZ.toLocal(ntp.timestamp))));
  //  s.replace("@@MIN@@", String(minute()));
  s.replace("@@NTPSRV@@", settings.timeserver);
  //s.replace("@@NTPSRV1@@", settings.timeserver1);
  s.replace("@@NTPINT@@", String(settings.interval));
  s.replace("@@SYNCSTATUS@@", ntp.timeIsSynced ? "OK" : "Overdue");
  s.replace("@@CLOCKNAME@@", settings.name);
  s.replace("@@UPDATERESPONSE@@", httpUpdateResponse);
  /* s.replace("@@DAY@@", String(day()));
    s.replace("@@MONTH@@", String(month()));
    s.replace("@@YEAR@@", String(year()));  */
  s.replace("@@VERSION@@", String(VERSION_MAJOR) + "." + String(VERSION_MINOR));
  s.replace("@@BUILDDATE@@", __DATE__);
  s.replace("@@BUILDTIME@@", __TIME__);

  s.replace("@@DST@@", settings.DST);
  s.replace("@@STD@@", settings.STD);


  if (settings.twelvehr == 1) {
  s.replace("@@12HRMODE@@", "checked");
  } else {
    s.replace("@@12HRMODE@@", "");
  }

  //dstDayofweek
  String dstDayofWeekTxt = "@@DSTDAYOFWEEKTXT@@";
  switch (settings.dstDayofweek) {
    case 0:  //default
      s.replace(dstDayofWeekTxt, "Sunday");
      settings.dstDayofweek = 1;
      break;
    case 1:
      s.replace(dstDayofWeekTxt, "Sunday");
      break;
    case 2:
      s.replace(dstDayofWeekTxt, "Monday");
      break;
    case 3:
      s.replace(dstDayofWeekTxt, "Tuesday");
      break;
    case 4:
      s.replace(dstDayofWeekTxt, "Wednesday");
      break;
    case 5:
      s.replace(dstDayofWeekTxt, "Thursday");
      break;
    case 6:
      s.replace(dstDayofWeekTxt, "Friday");
      break;
    case 7:
      s.replace(dstDayofWeekTxt, "Saturday");
      break;
  }
  s.replace("@@DSTDAYOFWEEK@@", String(settings.dstDayofweek));

  String dstWeekTxt = "@@DSTWEEKTXT@@";

  //dstWeek
  switch (settings.dstWeek) {
    case 1:
      s.replace(dstWeekTxt, "First");
      break;
    case 2:
      s.replace(dstWeekTxt, "Second");
      break;
    case 3:
      s.replace(dstWeekTxt, "Third");
      break;
    case 0:
      s.replace(dstWeekTxt, "Last");
      break;
  }
  s.replace("@@DSTWEEK@@", String(settings.dstWeek));



  //dstMonth
  String dstMonthTxt = "@@DSTMONTHTXT@@";
  switch (settings.dstMonth) {
    case 1:
      s.replace(dstMonthTxt, "January");
      break;
    case 2:
      s.replace(dstMonthTxt, "February");
      break;
    case 3:
      s.replace(dstMonthTxt, "March");
      break;
    case 4:
      s.replace(dstMonthTxt, "April");
      break;
    case 5:
      s.replace(dstMonthTxt, "May");
      break;
    case 6:
      s.replace(dstMonthTxt, "June");
      break;
    case 7:
      s.replace(dstMonthTxt, "July");
      break;
    case 8:
      s.replace(dstMonthTxt, "August");
      break;
    case 9:
      s.replace(dstMonthTxt, "September");
      break;
    case 10:
      s.replace(dstMonthTxt, "October");
      break;
    case 11:
      s.replace(dstMonthTxt, "November");
      break;
    case 12:
      s.replace(dstMonthTxt, "December");
      break;
  }
  s.replace("@@DSTMONTH@@", String(settings.dstMonth));

  s.replace("@@DSTHOUR@@", String(settings.dstHour));
  s.replace("@@DSTOFFSET@@", String(settings.dstOffset));

  // STD

  //STDDayofweek
  String stdDayOfWeekTxt = "@@STDDAYOFWEEKTXT@@";
  switch (settings.stdDayofweek) {
    case 0:
      s.replace(stdDayOfWeekTxt, "Sunday");
      settings.stdDayofweek = 1;
      break;
    case 1:
      s.replace(stdDayOfWeekTxt, "Sunday");
      break;
    case 2:
      s.replace(stdDayOfWeekTxt, "Monday");
      break;
    case 3:
      s.replace(stdDayOfWeekTxt, "Tuesday");
      break;
    case 4:
      s.replace(stdDayOfWeekTxt, "Wednesday");
      break;
    case 5:
      s.replace(stdDayOfWeekTxt, "Thursday");
      break;
    case 6:
      s.replace(stdDayOfWeekTxt, "Friday");
      break;
    case 7:
      s.replace(stdDayOfWeekTxt, "Saturday");
      break;
  }
  s.replace("@@STDDAYOFWEEK@@", String(settings.stdDayofweek));

  //stdWeek
  String stdWeekTxt = "@@STDWEEKTXT@@";
  switch (settings.stdWeek) {
    case 1:
      s.replace(stdWeekTxt, "First");
      break;
    case 2:
      s.replace(stdWeekTxt, "Second");
      break;
    case 3:
      s.replace(stdWeekTxt, "Third");
      break;
    case 0:
      s.replace(stdWeekTxt, "Last");
      break;
  }
  s.replace("@@STDWEEK@@", String(settings.stdWeek));



  //stdMonth
  String stdMonthTxt = "@@STDMONTHTXT@@";
  switch (settings.stdMonth) {
    case 1:
      s.replace(stdMonthTxt, "January");
      break;
    case 2:
      s.replace(stdMonthTxt, "February");
      break;
    case 3:
      s.replace(stdMonthTxt, "March");
      break;
    case 4:
      s.replace(stdMonthTxt, "April");
      break;
    case 5:
      s.replace(stdMonthTxt, "May");
      break;
    case 6:
      s.replace(stdMonthTxt, "June");
      break;
    case 7:
      s.replace(stdMonthTxt, "July");
      break;
    case 8:
      s.replace(stdMonthTxt, "August");
      break;
    case 9:
      s.replace(stdMonthTxt, "September");
      break;
    case 10:
      s.replace(stdMonthTxt, "October");
      break;
    case 11:
      s.replace(stdMonthTxt, "November");
      break;
    case 12:
      s.replace(stdMonthTxt, "December");
      break;
  }
  s.replace("@@STDMONTH@@", String(settings.stdMonth));

  s.replace("@@STDHOUR@@", String(settings.stdHour));
  s.replace("@@STDOFFSET@@", String(settings.stdOffset));

  // dim, bright hours
  s.replace("@@DIM@@", String(settings.dim));
  s.replace("@@BRIGHT@@", String(settings.bright));

  s.replace("@@ID@@", String(ESP.getChipId(), HEX));
  s.replace("@@SIGNAL@@", String(WiFi.RSSI()));

  s.replace("@@FUDGE@@", String(settings.fudge));
  s.replace("@@WIFICONNECTS@@", String(wificonnects));

  s.replace("@@DEBUG@@",  String(settings.DST) + " " + String(settings.dstWeek) + " " + String(settings.dstDayofweek) + " " + String(settings.dstMonth) + " " + String(settings.dstHour) + " " + String(settings.dstOffset) +
            "<br>" + String(settings.STD) + " " + String(settings.stdWeek) + " " + String(settings.stdDayofweek) +  " " + String(settings.stdMonth) + " " + String(settings.stdHour) + " " + String(settings.stdOffset) +
           "<br>Last Sync: " + String(ntp.getTimeDate(myTZ.toLocal(ntp.lastSync)))+ "<br>First Sync: " + String(ntp.getTimeDate(myTZ.toLocal(firstSync))) + "<br>LI: " + String(ntp.LI));
//  s.replace("@@TIMESERVER1@@", "<br>Current NTP Server 1: " + String(NTP.getNTPServer(0)));

//  if (sizeof(settings.timeserver1 > 0)) {
//   s.replace("@@TIMESERVER2@@", "<br>Current NTP Server 2: " + String(NTP.getNTPServer(1)));
//  }

  httpUpdateResponse = "";
  server.send(200, "text/html", s);
}

void handleForm() {


  if (!server.authenticate(www_username, String(ESP.getChipId(), HEX).c_str()) && clockMode == MODE_CLOCK) {
    return server.requestAuthentication();
  }


  String update_wifi = server.arg("update_wifi");
  String t_ssid = server.arg("ssid");
  String t_psk = server.arg("psk");
  String t_timeserver = server.arg("ntpsrv");
  t_timeserver.toCharArray(settings.timeserver, EEPROM_TIMESERVER_LENGTH, 0);
  //t_timeserver = server.arg("ntpsrv1");
  //t_timeserver.toCharArray(settings.timeserver1, EEPROM_TIMESERVER1_LENGTH, 0);
  if (update_wifi == "1") {
    settings.ssid = t_ssid;
    settings.psk = t_psk;
  }
  //dst
  server.arg("dst").toCharArray(settings.DST, EEPROM_DT_LENGTH, 0);
  server.arg("std").toCharArray(settings.STD, EEPROM_ST_LENGTH, 0);

  settings.dstWeek = server.arg("dstWeek").toInt();
  settings.stdWeek = server.arg("stdWeek").toInt();

  settings.dstDayofweek = server.arg("dstDayofweek").toInt();
  settings.stdDayofweek = server.arg("stdDayofweek").toInt();

  settings.dstMonth = server.arg("dstMonth").toInt();
  settings.stdMonth = server.arg("stdMonth").toInt();

  settings.dstHour = server.arg("dstHour").toInt();
  settings.stdHour = server.arg("stdHour").toInt();

  settings.dstOffset = server.arg("dstOffset").toInt();
  settings.stdOffset = server.arg("stdOffset").toInt();
  // settings.dstOffset = -240;
  // settings.stdOffset = -300;
  settings.dim = server.arg("dim").toInt();
  settings.bright = server.arg("bright").toInt();

  settings.fudge = server.arg("fudge").toInt();


  settings.twelvehr = server.arg("twelvehr").toInt();

  
  int syncInt = server.arg("ntpint").toInt();;
  if (syncInt < MINIMUM_INTERVAL) {
    syncInt = MINIMUM_INTERVAL;
  }
  
  settings.interval = syncInt;

  settings.name = server.arg("clockname");
  settings.name.replace("+", " ");

  httpUpdateResponse = "The configuration was updated, power cycle for DST changes to take effect.";

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Moved");

  settings.Save();


  server.handleClient();

  if (update_wifi == "1" || clockMode == MODE_SETUP) {
    delay(1000);
    setupWiFi();
  }

  
   ntp.ntpServerName = (char*)settings.timeserver;
  
  if (ntp.syncOffset != settings.interval) {
  ntp.syncOffset = settings.interval;
  ntp.syncTheTime();  //force an immediate re-sync so that the interval is updated
  }
  
  
  clockMode = MODE_CLOCK;

}

void handleCredits() {

if (!server.authenticate(www_username, String(ESP.getChipId(), HEX).c_str()) && clockMode == MODE_CLOCK) {
    return server.requestAuthentication();
  }
  
  server.send(200, "text/html", credits_page);


}

void setup() {

  //char hostname = char(ESP.getChipId());

  MDNS.begin(String(ESP.getChipId(), HEX).c_str());
  MDNS.addService("http", "tcp", 80);

  setupDisplay();
  pinMode(SETUP_PIN, INPUT_PULLUP);
  digitalWrite(SETUP_PIN, HIGH);

  server.begin();

  setupWiFi();
  //  setupTime();
  server.on("/", handleRoot);
  server.on("/form", handleForm);
  server.on("/credits", handleCredits);

  strcpy(www_password, ID.c_str());  //need this to get the pw to work for some reason with the update svr.

  httpUpdater.setup(&server, updatePath, www_username, www_password);  //ota   
 

  ntp.localPort = (uint16_t)random(0xC000, 0xFFFF);          //generate random port number in range C000-FFFF
  ntp.ntpServerName = (char*)settings.timeserver;
  ntp.syncOffset = settings.interval;
}

void loop() {
  server.handleClient();
 
    if (digitalRead(SETUP_PIN) == 0) {

    displayIP(false);
    delay(1000);
    displayIP(true);
    delay(1000);
    displayID();
    delay(1000);

    digitalWrite(SETUP_PIN, HIGH);
  }

 
  
  if (clockMode == MODE_CLOCK) {
  //  if (ntp.timeIsSynced) {

      if (millis() % 10 == 0 ) {
        ntp.addTime();
        displayClock();
        //connect wifi if not connected
        if (WiFi.status() != WL_CONNECTED) {
          delay(1);
          displayDash();
          setupSTA();
          wificonnects++;
          return;
        }
      }
      //   }
  //  }
  } else {
    //mode setup
    if (millis() % 1000 == 0) {
      switch (setupdisp) {
        case 0: displayID();
              setupdisp++;
              break;
        case 1:
              displayIP(false);
              setupdisp++;
              break;
        case 2:
              displayIP(true);
              setupdisp++;
              break;
        case 3:
              display8s();
              setupdisp = 0;
              break;
        }


    }

  }

}

void setupWiFi() {

  settings.Load();
  // Wait up to 5s for GPIO0 to go low to enter AP/setup mode.
  //displayBusy(0);
  //display8s();  // display test for 1 second (first 1000 ms)
  displayVersion();
  while (millis() < 5000) {
      if (millis() < 2000 && millis() > 1000) {
          display8s();
      }else if (millis() > 3000) {
          displayID();
        }

    
    if (digitalRead(SETUP_PIN) == 0 || !settings.ssid.length()) {
  
    return setupAP();
    }
    delay(50);
  }
  // stopDisplayBusy();
  setupSTA();
}

void setupSTA()
{
  char ssid[32];
  char psk[64];
  memset(ssid, 0, 32);
  memset(psk, 0, 64);
  //displayBusy(1);


  clockMode = MODE_CLOCK;
  WiFi.mode(WIFI_STA);
  settings.ssid.toCharArray(ssid, 32);
  settings.psk.toCharArray(psk, 64);
  WiFi.disconnect();  //this seems to help with the initial connect after a brief power interruption
  if (settings.psk.length()) {
    WiFi.begin(ssid, psk);
  } else {
    WiFi.begin(ssid);
  }
  
uint8_t conncount = 100;
 //if you don't succeed try try again
  while (WiFi.status() != WL_CONNECTED) {
    
    delay(100);
    if (conncount > 200) {
       WiFi.disconnect();
       displayDash();
       delay(1000);
       displayID();
       if (settings.psk.length()) {
            WiFi.begin(ssid, psk);
          } else {
            WiFi.begin(ssid);
          }
       conncount = 0;
    }

    
    conncount++;
  }
  // stopDisplayBusy();
  // displayDash();
  displayIP(false);
  delay(1000);
  displayIP(true);
  delay(1000);
}

void setupAP() {
  clockMode = MODE_SETUP;
  WiFi.mode(WIFI_AP);
  WiFi.softAP((String(WIFI_AP_NAME) + String(ESP.getChipId(), HEX)).c_str(), WPA_PSK);
  displayAP();
}



