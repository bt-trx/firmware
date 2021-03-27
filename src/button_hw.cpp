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

#include "settings.h"
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
  stateChanged = false;

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = currentTime;
  }

  if ((currentTime - lastDebounceTime) >= debounceDelay) {
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      stateChanged = true;
    }
  }

  checkForTripleClick(isPressedEdge(), currentTime);

  lastButtonState = reading;

}

void ButtonHW::checkForTripleClick(bool isPressedEdge, ulong currentTime) {
  static ulong lastClick = -1;
  static int clicks = 0;

  if (isPressedEdge) {
    if ((currentTime - lastClick < PTT_TRIPLE_CLICK_SPEED)) {
      // Second and Third Click
      clicks++;      
    } else {
      // First Click
      clicks = 1;
    }
    lastClick = currentTime;
  }

  if (clicks == 3) {
    tripleClick = true;
    clicks = 0;
  } else {
    tripleClick = false;
  }
}

/**
 * @brief Return current state of the button
 *
 * @return bool True if button is pressed
 */
bool ButtonHW::isPressed() {
  return !buttonState; // active low
}

/**
 * @brief Return current state of the button
 *
 * @return bool True if button is released
 */
bool ButtonHW::isReleased() {
  return buttonState; // active low
}

/**
 * @brief Returns true if the button was just pressed (edge), not the current
 * state
 *
 * @return bool
 */
bool ButtonHW::isPressedEdge() { return isPressed() && stateChanged; }

/**
 * @brief Returns true if the button was just released (edge), not the current
 * state
 *
 * @return bool
 */
bool ButtonHW::isReleasedEdge() { return isReleased() && stateChanged; }

/**
 * @brief Returns true if the button was just triple clicked, not the current
 * state
 *
 * @return bool
 */
bool ButtonHW::isTripleClick() { return tripleClick; }