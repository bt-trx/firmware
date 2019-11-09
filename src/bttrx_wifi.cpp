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

#ifdef ARDUINO

#include "bttrx_wifi.h"

AsyncWebServer server(80);

void onRequest(AsyncWebServerRequest *request)
{
	//Handle Unknown Request
	request->send(404);
}

String resultPage(bool error)
{
	String resultString = style + "<form>Firmware Update ";
	if (error) {
		resultString += "FAILED";
	} else {
		resultString += "OK";
	}
	resultString += ", rebooting...</form>";
	return resultString;
}

void BTTRX_WIFI::setup()
{
	Serial.println("Configuring access point...");

	WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);

	/*use mdns for host name resolution*/
	if (!MDNS.begin(WIFI_HOSTNAME)) { //http://bt-trx.local
		Serial.println("Error setting up MDNS responder!");
		while (1) {
			delay(1000);
		}
	}
	Serial.println("mDNS responder started");

	// respond to GET requests on URL /heap
	server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", String(ESP.getFreeHeap()));
	});

	/*return index page which is stored in serverIndex */
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response =
			request->beginResponse(200, "text/html", serverIndex);
		response->addHeader("Connection", "close");
		request->send(response);
	});
	/*handling uploading firmware file */
	server.on(
		"/update",
		HTTP_POST,
		[](AsyncWebServerRequest *request) {
			AsyncWebServerResponse *response =
				request->beginResponse(
					200,
					"text/html",
					resultPage(Update.hasError()));
			response->addHeader("Connection", "close");
			request->send(response);
			if (!Update.hasError()) {
				Serial.println("rebooting");
				ESP.restart();
			}
		},
		[](AsyncWebServerRequest *request,
		   String filename,
		   size_t index,
		   uint8_t *data,
		   size_t len,
		   bool final) {
			if (!index) {
				Serial.printf(
					"Update Start: %s\n", filename.c_str());
				if (!Update.begin(
					    (ESP.getFreeSketchSpace() - 0x1000) &
					    0xFFFFF000)) {
					Update.printError(Serial);
				}
			}
			if (!Update.hasError()) {
				if (Update.write(data, len) != len) {
					Update.printError(Serial);
				}
			}
			if (final) {
				if (Update.end(true)) {
					Serial.printf(
						"Update Success: %uB\n",
						index + len);
				} else {
					Update.printError(Serial);
				}
			}
		});

	// Catch-All Handler
	// Any request that can not find a Handler that canHandle it
	// ends in the callbacks below.
	server.onNotFound(onRequest);

	server.begin();
	MDNS.addService("http", "tcp", 80);

	Serial.printf(
		"Ready! Open http://%s.local in your browser\n", WIFI_HOSTNAME);
};

#endif //ARDUINO
