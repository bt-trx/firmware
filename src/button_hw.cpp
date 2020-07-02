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

#include "button_hw.h"

/**
 * @brief Helper class for handling button states
 *
 * @param pin
 */
ButtonHW::ButtonHW(uint32_t pin) : pin_(pin) { pinMode(pin_, INPUT); }

/**
 * @brief Update the button state
 *
 */
void ButtonHW::update() {
  bool reading = digitalRead(pin_);
  ulong currentTime = millis();
  state_changed = false;

  // If the switch changed, due to noise or pressing:
  if (reading != lastReading) {
    // reset the debouncing timer
    lastDebounceTime = currentTime;
  }

  if ((currentTime - lastDebounceTime) >= debounceDelay) {
    // if the button state has changed:
    ButtonState new_state = BTNSTATE_UNKNOWN;
    if (reading == HIGH) {
      new_state = BTNSTATE_RELEASED;
    }
    else {
      new_state = BTNSTATE_PRESSED;
    }

    if (new_state != button_state) {
      button_state = new_state;
      state_changed = true;
    }
  }

  checkForTripleClick(isPressedEdge(), currentTime);

  lastReading = reading;
}
