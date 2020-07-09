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

#include "button.h"
#include "settings.h"

/**
 * @brief Return current state of the button
 *
 * @return bool True if button is pressed
 */
bool Button::isPressed() {
  if (button_state == BTNSTATE_PRESSED) {
    return true;
  }
  return false;
}

/**
 * @brief Return current state of the button
 *
 * @return bool True if button is released
 */
bool Button::isReleased() {
  if (button_state == BTNSTATE_RELEASED) {
    return true;
  }
  return false;
}

/**
 * @brief Returns true if the button was just pressed (edge), not the current
 * state Button is active low
 *
 * @return bool
 */
bool Button::isPressedEdge() { return isPressed() && state_changed; }

/**
 * @brief Returns true if the button was just released (edge), not the current
 * state Button is active low
 *
 * @return bool
 */
bool Button::isReleasedEdge() { return isReleased() && state_changed; }

/**
 * @brief Returns true if the button was just clicked, not the current
 * state
 *
 * @return ClickType
 */
bool Button::isClicked() { return (clicks == 1); }

/**
 * @brief Returns true if the button was just triple-clicked, not the current
 * state
 *
 * @return ClickType
 */
bool Button::isTripleClicked() { return (clicks == 3); }

void Button::checkForClick(ulong last_change, ulong now) {

  // Account each additional click within click_delay
  if (isPressedEdge()) {
    click_count++;
  }

  // Button is released, report amount of clicks
  if (last_change + PTT_CLICK_SPEED < now &&
      button_state == BTNSTATE_RELEASED) {
    clicks = click_count;
    click_count = 0;
  }

  if (last_change + PTT_CLICK_SPEED + 10 < now &&
      button_state == BTNSTATE_PRESSED) {
    clicks = 255;
    click_count = 0;
  }
}
