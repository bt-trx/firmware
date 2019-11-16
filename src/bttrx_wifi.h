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
#include <WiFiAP.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>

#include "settings.h"
#include "bttrx_control.h"
#include "website.h"

extern const char *GIT_REVISION;

class BTTRX_WIFI {
    public:
	AsyncWebServer server = AsyncWebServer(80);
	void setup(BTTRX_CONTROL *);
	void handleSet(AsyncWebServerRequest *);
	void handleGet(AsyncWebServerRequest *);

    private:
	BTTRX_CONTROL *bttrx_control_;
	void firmwareUpdateResponse(AsyncWebServerRequest *);
	String resultPage(bool);
	void onRequest(AsyncWebServerRequest *);
};

#endif // ARDUINO
