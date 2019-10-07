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

#include "bttrx_wifi.h"
#include "settings.h"
#include "led.h"

String resultPage(bool result)
{
	String resultString = "Firmware Update ";
	if (result) {
		resultString += "OK";
	} else {
		resultString += "FAILED";
	}
	resultString += ", rebooting...";
	return resultString + style;
}

void BTTRX_WIFI::setup()
{
	Serial.println("Configuring access point...");

	WiFi.softAP(ssid, password);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);

	/*use mdns for host name resolution*/
	if (!MDNS.begin(host)) { //http://bt-trx.local
		Serial.println("Error setting up MDNS responder!");
		while (1) {
			delay(1000);
		}
	}
	Serial.println("mDNS responder started");

	/*return index page which is stored in serverIndex */
	server.on("/", HTTP_GET, []() {
		server.sendHeader("Connection", "close");
		server.send(200, "text/html", serverIndex);
	});
	/*handling uploading firmware file */
	server.on(
		"/update",
		HTTP_POST,
		[]() {
			server.sendHeader("Connection", "close");
			server.send(
				200,
				"text/plain",
				resultPage(Update.hasError()).c_str());
			ESP.restart();
		},
		[]() {
			HTTPUpload &upload = server.upload();
			if (upload.status == UPLOAD_FILE_START) {
				Serial.printf(
					"Update: %s\n",
					upload.filename.c_str());
				if (!Update.begin(
					    UPDATE_SIZE_UNKNOWN)) { //start with max available size
					Update.printError(Serial);
				}
			} else if (upload.status == UPLOAD_FILE_WRITE) {
				/* flashing firmware to ESP*/
				if (Update.write(
					    upload.buf, upload.currentSize) !=
				    upload.currentSize) {
					Update.printError(Serial);
				}
			} else if (upload.status == UPLOAD_FILE_END) {
				if (Update.end(
					    true)) { //true to set the size to the current progress
					Serial.printf(
						"Update Success: %u\nRebooting...\n",
						upload.totalSize);
				} else {
					Update.printError(Serial);
				}
			}
		});

	server.begin();
	MDNS.addService("http", "tcp", 80);

	Serial.printf("Ready! Open http://%s.local in your browser\n", host);
};

void BTTRX_WIFI::run()
{
	LED led_connected(PIN_LED_BLUE);
	LED led_busy(PIN_LED_GREEN);

	while (1) {
		led_connected.blink(500);
		led_busy.blink(500);
		server.handleClient();
	}
}
