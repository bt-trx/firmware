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
#include "Arduino.h"
#else
#include "arduino-mock/Arduino.h"
#endif

#include "led.h"

class PTT {
public:
  PTT(uint32_t ptt_pin, uint32_t led_pin);

  void checkForTimeout(uint32_t);
  void checkForDelayedOff();

  void on();
  void off();
  void delayed_off(uint32_t);
  void toggle(uint32_t = 0);
  bool getState() { return ptt_on_; };

private:
  int pin_;
  LED led;
  bool ptt_on_;

  ulong turn_on_time_;
  ulong turn_off_time_;
  uint32_t turn_off_delay_;
};
