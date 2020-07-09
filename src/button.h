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

enum ButtonState { BTNSTATE_UNKNOWN, BTNSTATE_RELEASED, BTNSTATE_PRESSED };

class Button {
public:
  bool isPressed();
  bool isReleased();
  bool isPressedEdge();
  bool isReleasedEdge();

  bool isClicked();
  bool isTripleClicked();
  bool isHeldDown() { return (clicks == 255); };

protected:
  ButtonState button_state = BTNSTATE_UNKNOWN;
  bool state_changed = false;
  ulong last_state_change_time = 0;

  void checkForClick(ulong last_state_change, ulong now);

  // Settings for Multiclick
  byte click_count = 0;
  byte clicks = 0;
};