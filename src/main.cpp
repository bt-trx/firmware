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

#include <Arduino.h>

#include "git_revision.h"
#include "settings.h"

#include "bttrx_fsm.h"
#include "bttrx_wifi.h"


WebServer server;
BTTRX_FSM bttrx_fsm;
BTTRX_WIFI bttrx_wifi;

void setup()
{
	// Set up GPIOs
	pinMode(PIN_BTN_0, INPUT);
	pinMode(PIN_LED_BLUE, OUTPUT);
	pinMode(PIN_LED_GREEN, OUTPUT);
	pinMode(PIN_PTT_IN, INPUT);
	pinMode(PIN_PTT_OUT, OUTPUT);
	pinMode(PIN_PTT_LED, OUTPUT);
	pinMode(PIN_BT_RESET, OUTPUT);

	// LEDs off
	digitalWrite(PIN_LED_BLUE, LOW);
	digitalWrite(PIN_LED_GREEN, LOW);
	digitalWrite(PIN_PTT_LED, LOW);
	// Don't trigger PTT
	digitalWrite(PIN_PTT_OUT, HIGH);
	// Get BT Module out of reset (active-low)
	digitalWrite(PIN_BT_RESET, HIGH);

	// Set up Serial ports
	SERIAL_DBG.begin(SERIAL_DBG_RATE);
	SERIAL_DBG.setTimeout(SERIAL_TIMEOUT);
	SERIAL_BT.begin(SERIAL_BT_RATE);
  SERIAL_BT.setTimeout(SERIAL_TIMEOUT);

// Wait for connection on debug Serial
#ifdef TEENSY32
	while (!SERIAL_DBG && millis() < DEBUG_CONNECT_TIMEOUT)
		;
#endif

	bttrx_fsm.setSerial(&SERIAL_BT, &SERIAL_DBG);

	// Print version information
	string header = "bt-trx v";
	header.append(GIT_REVISION);
	SERIAL_DBG.println(header.c_str());

	ulong startTime = millis();
	while (!digitalRead(PIN_BTN_0))
	{
		ulong currentTime = millis();
		if (startTime + BTN_PRESS_WIFI_MODE_TIMEOUT < currentTime)
		{
			bttrx_wifi.setup();
			bttrx_wifi.run();
		}
	}
}

void loop()
{
	bttrx_fsm.run();
}
