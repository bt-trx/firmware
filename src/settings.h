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

#ifdef ESP32
  // ESP32 Pin Info:
  // GPIO 34, 35: Input only
  // GPIO 6-11: SPI Flash, do not use
  // in general, pins > 17 are safe to use
  #define SERIAL_BT Serial2 // Default: RX: 16, TX: 17, RTS: 7, CTS: 8
  #define SERIAL_DBG Serial

  #define PIN_BTN_0 18  // active low
  #define PIN_LED_0 19  // active high
  #define PIN_LED_1 20  // active high
  #define PIN_PTT_IN 21
  #define PIN_PTT_OUT 22
  #define PIN_BT_RESET 23  // active low
#endif

#ifdef TEENSY32
  // Legacy bindings for dev-board v2 and v3
  #define SERIAL_BT Serial3
  #define SERIAL_DBG Serial

  #define PIN_BTN_0 23  // active low
  #define PIN_LED_0 3  // active high
  #define PIN_LED_1 4  // active high
  #define PIN_PTT_IN 5
  #define PIN_PTT_OUT 6
  #define PIN_BT_RESET 13  // active low
#endif

#define LED_BUSY PIN_LED_0
#define LED_CONNECTED PIN_LED_1

#define SERIAL_DBG_RATE 115200
#define SERIAL_BT_RATE 115200
#define SERIAL_TIMEOUT 100  // serial readline timeout
#define DEBUG_CONNECT_TIMEOUT 3000

#define SERIAL_DELIMITER '\n'
#define SERIAL_MAX_LINE_LENGTH 100
#define BT_SERIAL_TIMEOUT 1000 // ms  // Time to wait for answers from BT module
