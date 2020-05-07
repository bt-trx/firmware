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

Copyright (C) 2019 Christian Obersteiner (DL1COM), Andreas Müller (DC1MIL),
              2020 Florian Wolters (DF2ET)
Contact: bt-trx.com, mail@bt-trx.com
*/

#ifdef ARDUINO

#include "bttrx_display.h"
#include "bttrx_logo.h"

#include "OLEDDisplayUi.h"
#include "SH1106Wire.h"

extern const char *GIT_REVISION;
extern String hardwareVersion;

SH1106Wire display(0x3c, SDA, SCL); // ADDRESS, SDA, SCL

void BTTRX_DISPLAY::init() {
  display.init();
  display.flipScreenVertically();
  display.drawXbm(((display_width - bt_trx_logo_width) / 2), 0,
                  bt_trx_logo_width, bt_trx_logo_height, bt_trx_logo_bits);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString((display_width / 2), (display_height - 16), "bt-trx.com");
  display.display();
}

void BTTRX_DISPLAY::setStatusMessage(string message) {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString((display_width / 2), 0, "bt-trx v" + hardwareVersion);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, (display_height - 44), "FW:");
  display.drawString(24, (display_height - 44), GIT_REVISION);
  display.drawStringMaxWidth(0, (display_height - 30), display_width,
                             message.c_str());
  display.display();
}

void BTTRX_DISPLAY::setTransmitMessage(string message) {
  display.setColor(BLACK);
  display.fillRect(0, (display_height - 30), display_width, 30);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_16);
  display.drawString((display_width / 2), (display_height - 24),
                     message.c_str());
  display.display();
}

#endif // ARDUINO
