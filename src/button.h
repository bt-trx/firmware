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

#pragma once

enum ButtonState { BTNSTATE_UNKNOWN, BTNSTATE_RELEASED, BTNSTATE_PRESSED };

class Button {
public:
  bool isPressed();
  bool isReleased();
  bool isPressedEdge();
  bool isReleasedEdge();

protected:
  ButtonState button_state = BTNSTATE_UNKNOWN;
  bool state_changed = false;
};