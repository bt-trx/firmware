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
    : pin_(ptt_pin), led(led_pin), ptt_on_(false), turn_on_time_(-1),
      turn_off_time_(-1), turn_off_delay_(0) {
  pinMode(pin_, OUTPUT);
}

/**
 * @brief Check if the time-out-timer has been expired and turn off PTT in that
 * case
 *
 */
void PTT::checkForTimeout(uint32_t timeout_min) {
  if (timeout_min == 0) {
    return;
  } // Timeout disabled

  ulong timeout_ms = timeout_min * 60000; // minutes to ms
  if ((turn_on_time_ + timeout_ms) < millis()) {
    off();
  }
}

void PTT::checkForDelayedOff() {
  // Check repeatedly when the delay was reached and switch off
  if (turn_off_time_ != -1) {
    if ((turn_off_time_ + turn_off_delay_) <= millis()) {
      off();
    }
  }
}

/**
 * @brief Turn PTT output and PTT LED on
 *
 */
void PTT::on() {
  digitalWrite(pin_, LOW); // active low
  led.on();

  turn_on_time_ = millis();
  turn_off_time_ = -1;
  ptt_on_ = true;
}

/**
 * @brief Turn PTT output and PTT LED off
 *
 */
void PTT::off() {
  digitalWrite(pin_, HIGH); // active low
  led.off();
  turn_off_time_ = -1;

  ptt_on_ = false;
}

/**
 * @brief Toggle PTT
 *
 */
void PTT::toggle(uint32_t delay_ms) {
  if (ptt_on_) {
    delayed_off(delay_ms);
  } else {
    on();
#ifdef ARDUINO
    bttrx_display_.setTransmitMessage("<<< ON AIR >>>");
#endif // ARDUINO
  }
}

/**
 * @brief Turn PTT output and PTT LED off after the defined delay
 *
 * @param delay in ms
 */
void PTT::delayed_off(uint32_t delay_ms) {
  turn_off_delay_ = delay_ms;

  if (turn_off_delay_ == 0) {
    off();
#ifdef ARDUINO
    bttrx_display_.setTransmitMessage("idle");
#endif // ARDUINO
    return;
  }

  // Store the time when the command for turning off was received
  if (turn_off_time_ == -1) {
    turn_off_time_ = millis();
  }
}
