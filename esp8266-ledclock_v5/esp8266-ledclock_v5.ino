#define VERSION "1.5.35"

const char* www_username = "admin";
const char* updatePath = "/fwupload";

String ID = String(ESP.getChipId(), HEX);

char *www_password = new char[ID.length() + 1];



#include <Timezone.h>
#include "credits.h"

#include <EEPROM.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NTPClient.h>
#include <Time.h>

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
bool ipshown = false;
int wificonnects = 0;
uint8_t setupdisp = 0;
bool synced = false;

ESP8266WebServer server (80);

String httpUpdateResponse;

//time_t prevDisplay = 0;

void handleRoot() {

  if (!server.authenticate(www_username, String(ESP.getChipId(), HEX).c_str()) && clockMode == MODE_CLOCK) {
    return server.requestAuthentication();
  }

  String s = MAIN_page;
  s.replace("@@SSID@@", settings.ssid);
  s.replace("@@PSK@@", settings.psk);
  //  s.replace("@@TZ@@", String(settings.timezone));
  s.replace("@@TIME@@", String(NTP.getTimeDate(now())));
  //  s.replace("@@MIN@@", String(minute()));
  s.replace("@@NTPSRV@@", settings.timeserver);
  s.replace("@@NTPINT@@", String(settings.interval));
  s.replace("@@SYNCSTATUS@@", timeStatus() == timeSet ? "OK" : "Overdue");
  s.replace("@@CLOCKNAME@@", settings.name);
  s.replace("@@UPDATERESPONSE@@", httpUpdateResponse);
  /* s.replace("@@DAY@@", String(day()));
    s.replace("@@MONTH@@", String(month()));
    s.replace("@@YEAR@@", String(year()));  */
  s.replace("@@VERSION@@", VERSION);
  s.replace("@@BUILDDATE@@", __DATE__);
  s.replace("@@BUILDTIME@@", __TIME__);

  s.replace("@@DST@@", settings.DST);
  s.replace("@@STD@@", settings.STD);

  //dstDayofweek
  switch (settings.dstDayofweek) {
    case 0:  //default
      s.replace("@@DSTDAYOFWEEKTXT@@", "Sunday");
      settings.dstDayofweek = 1;
      break;
    case 1:
      s.replace("@@DSTDAYOFWEEKTXT@@", "Sunday");
      break;
    case 2:
      s.replace("@@DSTDAYOFWEEKTXT@@", "Monday");
      break;
    case 3:
      s.replace("@@DSTDAYOFWEEKTXT@@", "Tuesday");
      break;
    case 4:
      s.replace("@@DSTDAYOFWEEKTXT@@", "Wednesday");
      break;
    case 5:
      s.replace("@@DSTDAYOFWEEKTXT@@", "Thursday");
      break;
    case 6:
      s.replace("@@DSTDAYOFWEEKTXT@@", "Friday");
      break;
    case 7:
      s.replace("@@DSTDAYOFWEEKTXT@@", "Saturday");
      break;
  }
  s.replace("@@DSTDAYOFWEEK@@", String(settings.dstDayofweek));

  //dstWeek
  switch (settings.dstWeek) {
    case 1:
      s.replace("@@DSTWEEKTXT@@", "First");
      break;
    case 2:
      s.replace("@@DSTWEEKTXT@@", "Second");
      break;
    case 3:
      s.replace("@@DSTWEEKTXT@@", "Third");
      break;
    case 0:
      s.replace("@@DSTWEEKTXT@@", "Last");
      break;
  }
  s.replace("@@DSTWEEK@@", String(settings.dstWeek));



  //dstMonth
  switch (settings.dstMonth) {
    case 1:
      s.replace("@@DSTMONTHTXT@@", "January");
      break;
    case 2:
      s.replace("@@DSTMONTHTXT@@", "February");
      break;
    case 3:
      s.replace("@@DSTMONTHTXT@@", "March");
      break;
    case 4:
      s.replace("@@DSTMONTHTXT@@", "April");
      break;
    case 5:
      s.replace("@@DSTMONTHTXT@@", "May");
      break;
    case 6:
      s.replace("@@DSTMONTHTXT@@", "June");
      break;
    case 7:
      s.replace("@@DSTMONTHTXT@@", "July");
      break;
    case 8:
      s.replace("@@DSTMONTHTXT@@", "August");
      break;
    case 9:
      s.replace("@@DSTMONTHTXT@@", "September");
      break;
    case 10:
      s.replace("@@DSTMONTHTXT@@", "October");
      break;
    case 11:
      s.replace("@@DSTMONTHTXT@@", "November");
      break;
    case 12:
      s.replace("@@DSTMONTHTXT@@", "December");
      break;
  }
  s.replace("@@DSTMONTH@@", String(settings.dstMonth));

  s.replace("@@DSTHOUR@@", String(settings.dstHour));
  s.replace("@@DSTOFFSET@@", String(settings.dstOffset));

  // STD

  //STDDayofweek
  switch (settings.stdDayofweek) {
    case 0:
      s.replace("@@STDDAYOFWEEKTXT@@", "Sunday");
      settings.stdDayofweek = 1;
      break;
    case 1:
      s.replace("@@STDDAYOFWEEKTXT@@", "Sunday");
      break;
    case 2:
      s.replace("@@STDDAYOFWEEKTXT@@", "Monday");
      break;
    case 3:
      s.replace("@@STDDAYOFWEEKTXT@@", "Tuesday");
      break;
    case 4:
      s.replace("@@STDDAYOFWEEKTXT@@", "Wednesday");
      break;
    case 5:
      s.replace("@@STDDAYOFWEEKTXT@@", "Thursday");
      break;
    case 6:
      s.replace("@@STDDAYOFWEEKTXT@@", "Friday");
      break;
    case 7:
      s.replace("@@STDDAYOFWEEKTXT@@", "Saturday");
      break;
  }
  s.replace("@@STDDAYOFWEEK@@", String(settings.stdDayofweek));

  //stdWeek
  switch (settings.stdWeek) {
    case 1:
      s.replace("@@STDWEEKTXT@@", "First");
      break;
    case 2:
      s.replace("@@STDWEEKTXT@@", "Second");
      break;
    case 3:
      s.replace("@@STDWEEKTXT@@", "Third");
      break;
    case 0:
      s.replace("@@STDWEEKTXT@@", "Last");
      break;
  }
  s.replace("@@STDWEEK@@", String(settings.stdWeek));



  //stdMonth
  switch (settings.stdMonth) {
    case 1:
      s.replace("@@STDMONTHTXT@@", "January");
      break;
    case 2:
      s.replace("@@STDMONTHTXT@@", "February");
      break;
    case 3:
      s.replace("@@STDMONTHTXT@@", "March");
      break;
    case 4:
      s.replace("@@STDMONTHTXT@@", "April");
      break;
    case 5:
      s.replace("@@STDMONTHTXT@@", "May");
      break;
    case 6:
      s.replace("@@STDMONTHTXT@@", "June");
      break;
    case 7:
      s.replace("@@STDMONTHTXT@@", "July");
      break;
    case 8:
      s.replace("@@STDMONTHTXT@@", "August");
      break;
    case 9:
      s.replace("@@STDMONTHTXT@@", "September");
      break;
    case 10:
      s.replace("@@STDMONTHTXT@@", "October");
      break;
    case 11:
      s.replace("@@STDMONTHTXT@@", "November");
      break;
    case 12:
      s.replace("@@STDMONTHTXT@@", "December");
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
            "<br>Last Sync: " + String(NTP.getTimeDate(NTP.getLastSync())) + "<br>First Sync: " + String(NTP.getTimeDate(NTP.getFirstSync())) + "<br>Current NTP Server: " + String(NTP.getNTPServer(0)));

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


  // String tz = server.arg("timezone");

  //  if (tz.length()) {
  //    settings.timezone = tz.toInt();
  //  }

  /*
    time_t newTime = time(nullptr);
    if (newTime) {
      setTime(newTime);
    }   */



  String syncInt = server.arg("ntpint");
  settings.interval = syncInt.toInt();

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

  // only re-init if something changed here
  if (settings.interval != NTP.getPollingInterval()) {
  
  NTP.init(settings.timeserver, UTC);

  } 
  NTP.setPollingInterval(settings.interval);
  NTP.setNTPServer(settings.timeserver);
  
  
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
  pinMode(SETUP_PIN, INPUT);
  digitalWrite(SETUP_PIN, HIGH);

  server.begin();

  setupWiFi();
  //  setupTime();
  server.on("/", handleRoot);
  server.on("/form", handleForm);
  server.on("/credits", handleCredits);

  strcpy(www_password, ID.c_str());  //need this to get the pw to work for some reason with the update svr.

  httpUpdater.setup(&server, updatePath, www_username, www_password);  //ota   
 
  
  //httpUpdater2.setup(&server); //backdoor - remove

  NTP.init(settings.timeserver, UTC);
  NTP.setPollingInterval(settings.interval);

  // this is supposed to be run when the server is synced 
  NTP.onSyncEvent([](NTPSyncEvent_t ntpEvent) {
    switch (ntpEvent) {
    case NTP_EVENT_INIT:
      break;
    case NTP_EVENT_STOP:
      break;
    case NTP_EVENT_NO_RESPONSE:
     // Serial.printf("NTP server not reachable.\n");
      break;
    case NTP_EVENT_SYNCHRONIZED:
     // Serial.printf("Got NTP time: %s\n", NTP.getTimeDate(NTP.getLastSync()));
     synced = true;
      break;
    }
  });

}

void loop() {
  server.handleClient();
  if (digitalRead(SETUP_PIN) == 0 && !ipshown) {

    displayIP(false);
    delay(1000);
    displayIP(true);
    delay(1000);
    displayID();
    delay(1000);
    ipshown = true;
  //   digitalWrite(SETUP_PIN, LOW);
  }
  if (clockMode == MODE_CLOCK) {
    if (timeStatus() != timeNotSet) {

      if (millis() % 10 == 0 ) {
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
    }
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
  displayID();
  while (millis() < 5000) {
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
  if (settings.psk.length()) {
    WiFi.begin(ssid, psk);
  } else {
    WiFi.begin(ssid);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  // stopDisplayBusy();
  // displayDash();
  displayIP(false);
  delay(1000);
  displayIP(true);
}

void setupAP() {
  clockMode = MODE_SETUP;
  WiFi.mode(WIFI_AP);
  WiFi.softAP((String(WIFI_AP_NAME) + String(ESP.getChipId(), HEX)).c_str(), WPA_PSK);
  displayAP();
}



