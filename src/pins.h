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
// Bindings for dev-board v4 and newer
// ESP32 Pin Info:
// GPIO 6-11: Do not use! (onboard SPI Flash)
// GPIO 34-39: Input only
// GPIO 32-39: ADC1
// ADC2 is not available when using Wifi!
// in general, pins > 17 are safe to use
#define PIN_BT_RESET 4   // Active Low
#define PIN_LED_BLUE 25  // Active High
#define PIN_LED_GREEN 26 // Active High
#define PIN_BTN_0 18     // Active Low
#define PIN_PTT_IN 32    // Active Low
#define PIN_PTT_OUT 33   // Active Low
#define PIN_PTT_LED 13   // Active High
#define PIN_WT32_RX 16   // Serial2 Rx
#define PIN_WT32_TX 17   // Serial2 Tx
#define PIN_HW_VER 34    // ADC1 CH6
#define PIN_VOX_IN 35    // ADC1 CH7
#define SERIAL_DBG Serial
#define SERIAL_BT Serial2 // Default: RX: 16, TX: 17, RTS: 7, CTS: 8
#endif

#ifdef TEENSY32
// Legacy bindings for dev-board v2 and v3
#define SERIAL_BT Serial3
#define SERIAL_DBG Serial

#define PIN_BTN_0 23    // active low
#define PIN_LED_BLUE 3  // active high
#define PIN_LED_GREEN 4 // active high
#define PIN_PTT_IN 5    // active Low
#define PIN_PTT_OUT 6   // active Low
#define PIN_BT_RESET 13 // active low
#endif