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

Copyright (C) 2019-2020 Christian Obersteiner (DL1COM), Andreas Müller (DC1MIL)
and contributors
Contact: bt-trx.com, mail@bt-trx.com
*/

#pragma once

#include "pins.h"

#define SERIAL_DBG_RATE 115200
#define SERIAL_BT_RATE 115200
#define SERIAL_TIMEOUT 10 // ms  // serial readline timeout
#define SERIAL_DELIMITER '\n'
#define SERIAL_MAX_LINE_LENGTH 200
#define BT_SERIAL_TIMEOUT 100 // ms  // Time to wait for answers from BT module

#define INQUIRY_DURATION "5" // *1.28s
#define BLE_SCAN_DURATION 1  // s
#define BLE_SCAN_INTERVAL 5  // s

#define BTN_PRESS_WIFI_MODE_TIMEOUT 5000 // ms

#define WIFI_HOSTNAME "bt-trx"
#define WIFI_SSID_PREFIX "bt-trx"
#define WIFI_PASSWORD "bt-trx73" // minimum 8 chars

#define BD_ADDR_OUI_ANYTONE "00:1b:10" // Anytone Bluetooth PTT BP-01

#define PTT_CLICK_SPEED 250        // ms
#define PTT_TIMEOUT_WILLIMODE 1000 // ms
#define TONE_1750HZ_LENGTH 1000    // ms (min. 600 ms)

#define CALLSIGN_LENGTH 6 // Max length of callsign for BT/WiFi identification

// Teensy specific
#define DEBUG_CONNECT_TIMEOUT 3000 // Needed for Teensy startup
