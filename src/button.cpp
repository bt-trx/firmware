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

/**
 * @brief Helper class for handling button states
 *
 * @param pin
 */
Button::Button(uint32_t pin) : pin_(pin)
{
	pinMode(pin_, INPUT);
}

/**
 * @brief Update the button state
 * 
 */
void Button::update()
{
	bool reading      = digitalRead(pin_);
	ulong currentTime = millis();
	stateChanged      = false;

	// If the switch changed, due to noise or pressing:
	if (reading != lastButtonState) {
		// reset the debouncing timer
		lastDebounceTime = currentTime;
	}

	if ((currentTime - lastDebounceTime) >= debounceDelay) {
		// if the button state has changed:
		if (reading != buttonState) {
			buttonState  = reading;
			stateChanged = true;
		}
	}
	lastButtonState = reading;
}

/**
 * @brief Return current state of the button
 *
 * @return bool True if button is pressed
 */
bool Button::isPressed()
{
	return !buttonState; // active low
}

/**
 * @brief Return current state of the button
 * 
 * @return bool True if button is released
 */
bool Button::isReleased()
{
	return buttonState; // active low
}

/**
 * @brief Returns true if the button was just pressed (edge), not the current state
 * Button is active low
 *
 * @return bool
 */
bool Button::isPressedEdge()
{
	return isPressed() && stateChanged;
}

/**
 * @brief Returns true if the button was just released (edge), not the current state
 * Button is active low
 * 
 * @return bool
 */
bool Button::isReleasedEdge()
{
	return isReleased() && stateChanged;
}
