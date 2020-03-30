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

#include "button_ble.h"

/**
 * @brief Update the button state
 *
 */
void ButtonBLE::update() {
  state_changed = false;
  if (next_state != button_state) {
    button_state = next_state;
    state_changed = true;
  }
}

/**
 * @brief Update the button state
 *
 */
void ButtonBLE::setPressed() { next_state = BTNSTATE_PRESSED; }

/**
 * @brief Update the button state
 *
 */
void ButtonBLE::setReleased() { next_state = BTNSTATE_RELEASED; }
