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

#ifdef ARDUINO
#include "Arduino.h"
#else
#include "arduino-mock/Arduino.h"
#endif

#include "ptt.h"
#include "serialwrapper.h"

class Tone1750 {
public:
  Tone1750(PTT *, SerialWrapper *);
  void update();
  bool isActive();
  void send();

private:
  PTT *_ptt_button = 0;
  SerialWrapper *_serial = 0;
  ulong _start_time = 0;
  bool _active = false;

  void stop();
};