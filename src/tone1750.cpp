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

#include "tone1750.h"
#include "settings.h"

Tone1750::Tone1750(PTT *ptt_button, SerialWrapper *serialwrapper)
    : _ptt_button(ptt_button), _serial(serialwrapper) {}

void Tone1750::update() {
  ulong now = millis();

  if (_start_time + TONE_1750HZ_LENGTH < now) {
    stop();
  }
}

bool Tone1750::isActive() { return _active; }

void Tone1750::send() {
  _ptt_button->on();
  _active = true;
  _start_time = millis();

  // TODO Find correct tone and how to determine/vary length to play
  _serial->println("PLAYSOUND 3 3 0 goodbye.adpcm");
}

void Tone1750::stop() {
  _active = false;
  _ptt_button->off();
  // TODO DO WE HAVE TO STOP PLAYSOUND?
}
