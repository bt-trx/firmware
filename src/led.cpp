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

#include "led.h"

/**
 * @brief Helper class for better readability when controlling the LEDs and
 * enable a convenient way for blinking
 *
 * @param pin
 */
LED::LED(uint32_t pin) : pin_(pin) { pinMode(pin_, OUTPUT); }

void LED::on() { digitalWrite(pin_, HIGH); }

void LED::off() { digitalWrite(pin_, LOW); }

void LED::blink(uint32_t interval) {
  if ((millis() - last_toggle_) > interval) {
    toggle();
    last_toggle_ = millis();
  }
}

void LED::toggle() { digitalWrite(pin_, !digitalRead(pin_)); }
