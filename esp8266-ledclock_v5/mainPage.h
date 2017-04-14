
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>

 <meta name="viewport" content="initial-scale=1">
 <style>
 body {font-family: helvetica,arial,sans-serif;}
 table {border-collapse: collapse; border: 1px solid black;}
 td {padding: 0.25em;}
 .title { font-size: 2em; font-weight: bold;}
 .name {padding: 0.5em;}
 .heading {font-weight: bold; background: #c0c0c0; padding: 0.5em;}
 .update {color: #dd3333; font-size: 0.75em;}
 </style>
</head><body>
<div class=title>NTP Clock @@VERSION@@</div>
<div class=name>Location: @@CLOCKNAME@@</div>
<div class=name>Built @@BUILDDATE@@ @@BUILDTIME@@</div>


<form method="post" action="/form">
<table>
<tr><td colspan=2 class=heading>Status</td></tr>
<tr><td>Current time:</td><td><b>@@TIME@@</b></td></tr>
<tr><td>Sync Status:</td><td>@@SYNCSTATUS@@</td></tr>
<tr><td>Wifi Reconnects:</td><td>@@WIFICONNECTS@@</td></tr>
<tr><td>Signal:</td><td>@@SIGNAL@@ db</td></tr>
<tr><td>Clock ID:</td><td>@@ID@@</td></tr>
<tr><td>Name:</td><td><input type=text name="clockname" value="@@CLOCKNAME@@"></td></tr>


<tr><td colspan=2 class=heading>WiFi Setup</td></tr>
<tr><td>SSID:</td><td><input type=text name="ssid" value="@@SSID@@"></td></tr>
<tr><td>PSK:</td><td><input type=text name="psk" value=""></td></tr>
<tr><td colspan=2>Update Wifi config:<input type=checkbox name=update_wifi value=1></td></tr>

<tr><td colspan=2 class=heading>Time Setup</td></tr>
<tr><td>NTP Server:</td><td><input type=text name="ntpsrv" value="@@NTPSRV@@"></td></tr>
<tr><td>Sync Interval:</td><td><input type=text name="ntpint" value="@@NTPINT@@">s</td></tr>
<tr><td>DST:</td><td><input type=text name="dst" value="@@DST@@" size=3></td>
<tr><tr><td>
DST Week:</td><td>
<select name=dstWeek>
  <option value="@@DSTWEEK@@" selected>@@DSTWEEKTXT@@</option>
  <option value="1">First</option>
  <option value="2">Second</option>
  <option value="3">Third</option>
  <option value="4">Fourth</option>
  <option value="0">Last</option>
</select>
</td>
</tr>
<tr>
<td>
DST Day:</td>
<td>
<select name=dstDayofweek>
  <option value="@@DSTDAYOFWEEK@@" selected>@@DSTDAYOFWEEKTXT@@</option>
  <option value="1">Sunday</option>
  <option value="2">Monday</option>
  <option value="3">Tuesday</option>
  <option value="4">Wednesday</option>
  <option value="5">Thursday</option>
  <option value="6">Friday</option>
  <option value="7">Saturday</option>
</select>
</td>
</tr>
<tr>
<td>
DST Month:</td>
<td>
<select name=dstMonth>
  <option value="@@DSTMONTH@@" selected>@@DSTMONTHTXT@@</option>
  <option value="1">January</option>
  <option value="2">February</option>
  <option value="3">March</option>
  <option value="4">April</option>
  <option value="5">May</option>
  <option value="6">June</option>
  <option value="7">July</option>
  <option value="8">August</option>
  <option value="9">September</option>
  <option value="10">October</option>
  <option value="11">November</option>
  <option value="12">December</option>
</select>
</td>
</tr>
<tr>
<td>
DST Hour:</td>
<td>
<select name=dstHour>
  <option value="@@DSTHOUR@@" selected>@@DSTHOUR@@</option>
  <option value="0">0</option>
  <option value="1">1</option>
  <option value="2">2</option>
  <option value="3">3</option>
  <option value="4">4</option>
  <option value="5">5</option>
  <option value="6">6</option>
  <option value="7">7</option>
  <option value="8">8</option>
  <option value="9">9</option>
  <option value="10">10</option>
  <option value="11">11</option>
  <option value="12">12</option>
  <option value="13">13</option>
  <option value="14">14</option>
  <option value="15">15</option>
  <option value="16">16</option>
  <option value="17">17</option>
  <option value="18">18</option>
  <option value="19">19</option>
  <option value="20">20</option>
  <option value="21">21</option>
  <option value="22">22</option>
  <option value="23">23</option>
</select>
</td>
</tr>
<tr>
<td>
DST Offset:
</td>
<td>
<input name=dstOffset type=text value=@@DSTOFFSET@@>m
</td>
</tr>

<tr><td>STD:</td><td><input type=text name="std" value="@@STD@@" size=3></td>
<tr><tr><td>
STD Week:</td><td>
<select name=stdWeek>
  <option value="@@STDWEEK@@" selected>@@STDWEEKTXT@@</option>
  <option value="1">First</option>
  <option value="2">Second</option>
  <option value="3">Third</option>
  <option value="0">Last</option>
</select>
</td>
</tr>
<tr>
<td>
STD Day:</td>
<td>
<select name=stdDayofweek>
  <option value="@@STDDAYOFWEEK@@" selected>@@STDDAYOFWEEKTXT@@</option>
  <option value="1">Sunday</option>
  <option value="2">Monday</option>
  <option value="3">Tuesday</option>
  <option value="4">Wednesday</option>
  <option value="5">Thursday</option>
  <option value="6">Friday</option>
  <option value="7">Saturday</option>
</select>
</td>
</tr>
<tr>
<td>
STD Month:</td>
<td>
<select name=stdMonth>
  <option value="@@STDMONTH@@" selected>@@STDMONTHTXT@@</option>
  <option value="1">January</option>
  <option value="2">February</option>
  <option value="3">March</option>
  <option value="4">April</option>
  <option value="5">May</option>
  <option value="6">June</option>
  <option value="7">July</option>
  <option value="8">August</option>
  <option value="9">September</option>
  <option value="10">October</option>
  <option value="11">November</option>
  <option value="12">December</option>
</select>
</td>
</tr>
<tr>
<td>
STD Hour:</td>
<td>
<select name=stdHour>
  <option value="@@STDHOUR@@" selected>@@STDHOUR@@</option>
  <option value="0">0</option>
  <option value="1">1</option>
  <option value="2">2</option>
  <option value="3">3</option>
  <option value="4">4</option>
  <option value="5">5</option>
  <option value="6">6</option>
  <option value="7">7</option>
  <option value="8">8</option>
  <option value="9">9</option>
  <option value="10">10</option>
  <option value="11">11</option>
  <option value="12">12</option>
  <option value="13">13</option>
  <option value="14">14</option>
  <option value="15">15</option>
  <option value="16">16</option>
  <option value="17">17</option>
  <option value="18">18</option>
  <option value="19">19</option>
  <option value="20">20</option>
  <option value="21">21</option>
  <option value="22">22</option>
  <option value="23">23</option>
</select>
</td>
</tr>
<tr>
<td>
STD Offset:
</td>
<td>
<input name=stdOffset type=text value=@@STDOFFSET@@>m
</td>
</tr>
<tr>
<td>
Dim Start Hour:</td>
<td>
<select name=dim>
  <option value="@@DIM@@" selected>@@DIM@@</option>
  <option value="0">0</option>
  <option value="1">1</option>
  <option value="2">2</option>
  <option value="3">3</option>
  <option value="4">4</option>
  <option value="5">5</option>
  <option value="6">6</option>
  <option value="7">7</option>
  <option value="8">8</option>
  <option value="9">9</option>
  <option value="10">10</option>
  <option value="11">11</option>
  <option value="12">12</option>
  <option value="13">13</option>
  <option value="14">14</option>
  <option value="15">15</option>
  <option value="16">16</option>
  <option value="17">17</option>
  <option value="18">18</option>
  <option value="19">19</option>
  <option value="20">20</option>
  <option value="21">21</option>
  <option value="22">22</option>
  <option value="23">23</option>
</select>
</td>
</tr>

<tr>
<td>
Dim End Hour:</td>
<td>
<select name=bright>
  <option value="@@BRIGHT@@" selected>@@BRIGHT@@</option>
  <option value="0">0</option>
  <option value="1">1</option>
  <option value="2">2</option>
  <option value="3">3</option>
  <option value="4">4</option>
  <option value="5">5</option>
  <option value="6">6</option>
  <option value="7">7</option>
  <option value="8">8</option>
  <option value="9">9</option>
  <option value="10">10</option>
  <option value="11">11</option>
  <option value="12">12</option>
  <option value="13">13</option>
  <option value="14">14</option>
  <option value="15">15</option>
  <option value="16">16</option>
  <option value="17">17</option>
  <option value="18">18</option>
  <option value="19">19</option>
  <option value="20">20</option>
  <option value="21">21</option>
  <option value="22">22</option>
  <option value="23">23</option>
</select>
</td>
</tr>
<tr><td>Fudge Amount:</td><td><input type=text name="fudge" value="@@FUDGE@@" size=5>s</td></tr>
<tr><td>12 HR mode?:</td><td><input type=checkbox name="twelvehr" value=1 @@12HRMODE@@></td></tr>
</table>
<p/>
<input type="submit" value="Update">
</form>
<div class="update">@@UPDATERESPONSE@@</div>
<p>
@@DEBUG@@
@@LASTIP@@
<p>
<a href="/">Refresh Page</a>
<p>
<a href="/credits">Credits</a>
</body>
</html>
)=====";

