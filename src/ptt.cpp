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

#include "ptt.h"

/**
 * @brief Helper class for control of the PTT output
 * Automatically controls PTT LED
 *
 * @param ptt_pin
 * @param led_pin
 */
PTT::PTT(uint32_t ptt_pin, uint32_t led_pin)
	: pin_(ptt_pin), led(led_pin), state_(false), turn_off_time(-1)
{
	pinMode(pin_, OUTPUT);
}

/**
 * @brief Turn PTT output and PTT LED on
 * 
 */
void PTT::on()
{
	digitalWrite(pin_, LOW); // active low
	led.on();
	turn_off_time = -1;

	state_ = true;
}

/**
 * @brief Turn PTT output and PTT LED off
 * 
 */
void PTT::off()
{
	digitalWrite(pin_, HIGH); // active low
	led.off();
	turn_off_time = -1;

	state_ = false;
}

/**
 * @brief Toggle PTT
 * 
 */
void PTT::toggle()
{
	if (state_) {
		off();
	} else {
		on();
	}
}

/**
 * @brief Turn PTT output and PTT LED off after the defined delay
 * 
 * @param delay in ms
 */
void PTT::delayed_off(uint32_t delay_ms)
{
	if (turn_off_time == -1) {
		turn_off_time = millis();
	} else {
		if ((turn_off_time + delay_ms) <= millis()) {
			off();
		}
	}
}
