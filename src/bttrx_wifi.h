/*
This file is part of bt-trx

bt-trx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

bt-trx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Copyright (C) 2019 Christian Obersteiner (DL1COM), Andreas Müller (DC1MIL)
Contact: bt-trx.com, mail@bt-trx.com
*/

#pragma once

#ifdef ARDUINO

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFiAP.h>
#include <ESPmDNS.h>
#include <Update.h>

#include "settings.h"

extern WebServer server;
extern const char *GIT_REVISION;

class BTTRX_WIFI {
    public:
	void setup();
	void run();
};

/* Style */
static const String style =
	"<style>"
	"body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
	"form{background:#fff;max-width:480px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
	"input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
	"input{border:0;padding:0 15px}"
	".btn{background:#3498db;color:#fff;cursor:pointer}"
	"</style>";

/* Server Index Page */
static const String serverIndex =
	"<form method='POST' action='/update' enctype='multipart/form-data' id='upload_form'>"
	"<table align='center'>"
	"<tr>"
	"<td>"
	"<center><font size=6><b>bt-trx<br>Firmware Update</b></font></center>"
	"<br><br>"
	"</td>"
	"</tr>"
	"<tr>"
	"<td><center><input type='file' name='update' id='file'></center><br></td>"
	"</tr>"
	"<tr>"
	"<td><input type='submit' class=btn value='Update'></td>"
	"</tr>"
	"<tr>"
	"<td><center>Current Firmware Version: " +
	String(GIT_REVISION) +
	"</center></td>"
	"</tr>"
	"</table>"
	"</form>" +
	style;

#endif // ARDUINO
