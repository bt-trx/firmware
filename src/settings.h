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

#include "pins.h"

#define SERIAL_DBG_RATE 115200
#define SERIAL_BT_RATE 115200
#define SERIAL_TIMEOUT 10 //ms  // serial readline timeout
#define SERIAL_DELIMITER '\n'
#define SERIAL_MAX_LINE_LENGTH 100
#define BT_SERIAL_TIMEOUT 100 // ms  // Time to wait for answers from BT module

#define INQUIRY_DURATION "5" // *1.28s
#define BLE_SCAN_DURATION 1  // s
#define BLE_SCAN_INTERVAL 5  // s

#define BTN_PRESS_WIFI_MODE_TIMEOUT 5000 // ms

#define WIFI_HOSTNAME "bt-trx"
#define WIFI_SSID "bt-trx"
#define WIFI_PASSWORD "bt-trx73" // minimum 8 chars

// Answers for AT Commands like AT+CGMM
#define AT_OS_REVISION "1.0"
#define AT_SERIAL_NUMBER "123456"
#define AT_MANUFACTURER_IDENTIFICATION "bt-trx"
#define AT_MODEL_IDENTIFICATION "bt-trx"
#define AT_IMSI "262735573557355"
#define AT_IMEI "123123123123123"

// Teensy specific
#define DEBUG_CONNECT_TIMEOUT 3000 // Needed for Teensy startup
