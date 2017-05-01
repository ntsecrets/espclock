
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
<tr><td>Signal:</td><td>@@SIGNAL@@ db</td></tr>
<tr><td>Clock ID:</td><td>@@ID@@</td></tr>
<tr><td>Name:</td><td><input type=text name="clockname" value="@@CLOCKNAME@@"></td></tr>


<tr><td colspan=2 class=heading>WiFi Setup</td></tr>
<tr><td>SSID:</td><td><input type=text name="ssid" maxlength="32" autocorrect="off" autocapitalize="off" spellcheck="false" value="@@SSID@@"></td></tr>
<tr><td>PSK:</td><td><input type=text name="psk" maxlength="64" autocomplete="off" autocorrect="off" autocapitalize="off" spellcheck="false" value=""></td></tr>
<tr><td colspan=2>Update Wifi config:<input type=checkbox name=update_wifi value=1></td></tr>

<tr><td colspan=2 class=heading>Time Setup</td></tr>
<tr><td>NTP Server:</td><td><input type=text name="ntpsrv" value="@@NTPSRV@@"></td></tr>
<tr><td>Sync Interval:</td><td><input type=text name="ntpint" value="@@NTPINT@@">s</td></tr>
<tr><td>DST:</td><td><input type=text name="dst" value="@@DST@@" size=3></td>
<tr><tr><td>
DST Week:</td><td>
@@DSTWEEK@@
</td>
</tr>
<tr>
<td>
DST Day:</td>
<td>
@@DSTDAYOFWEEK@@
</td>
</tr>
<tr>
<td>
DST Month:</td>
<td>
@@DSTMONTH@@
</td>
</tr>
<tr>
<td>
DST Hour:</td>
<td>
@@DSTHOUR@@
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
@@STDWEEK@@
</td>
</tr>
<tr>
<td>
STD Day:</td>
<td>
@@STDDAYOFWEEK@@
</td>
</tr>
<tr>
<td>
STD Month:</td>
<td>
@@STDMONTH@@
</td>
</tr>
<tr>
<td>
STD Hour:</td>
<td>
@@STDHOUR@@
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
Dim Mode:</td>
<td>
@@DIMMODE@@
</td>
</tr>
<tr>
<td>
Dim Start Hour:</td>
<td>
@@DIM@@
</td>
</tr>
<tr>
<td>
Dim End Hour:</td><td>
@@BRIGHT@@
</td>
</tr>
<tr><td>Fudge Amount:</td><td><input type=text name="fudge" value="@@FUDGE@@" size=5>s</td></tr>
<tr><td>12 HR mode?:</td><td><input type=checkbox name="twelvehr" value=1 @@12HRMODE@@></td></tr>
<tr><td>Sync Indicator?:</td><td><input type=checkbox name="syncind" value=1 @@SYNCIND@@></td></tr>
<tr><td>Center Dot?:</td><td><input type=checkbox name="centerdot" value=1 @@CENTERDOT@@></td></tr>
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
<a href="/fwupload">Firmware Upload</a>
<p>
<a href="https://github.com/ntsecrets/espclock/wiki/Credits">Credits</a><br>
For support and sales information, go to <a href="http://www.espclock.com">www.espclock.com</a> or email sales@espclock.com.<br>
</body>
</html>
)=====";


