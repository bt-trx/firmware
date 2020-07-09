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

#include "button_hw.h"

/**
 * @brief Helper class for handling button states
 *
 * @param pin
 */
ButtonHW::ButtonHW(uint32_t pin, bool activeLow)
    : pin_(pin), activeLow_(activeLow) {
  pinMode(pin_, INPUT);
}

/**
 * @brief Update the button state
 *
 */
void ButtonHW::update() {
  ulong now = millis();
  bool reading = digitalRead(pin_);

  // If button is active high, invert reading
  if (!activeLow_) {
    reading = !reading;
  }

  // If the switch changed, due to noise or pressing:
  if (reading != last_reading) {
    // reset the debouncing timer
    last_debounce_time = now;
  }

  ButtonState new_state = BTNSTATE_UNKNOWN;
  if (reading == HIGH) {
    new_state = BTNSTATE_RELEASED;
  } else {
    new_state = BTNSTATE_PRESSED;
  }

  // Reading has settled
  state_changed = false;
  if (last_debounce_time + debounce_delay < now && button_state != new_state) {
    button_state = new_state;
    state_changed = true;
    last_state_change_time = now;
  }
  last_reading = reading;

  checkForClick(last_state_change_time, now);
}
