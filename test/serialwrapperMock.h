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

#include "../src/serialwrapper.h"

class SerialWrapperMock : public SerialWrapperInterface {
  public:
    MOCK_METHOD1(println, size_t(const char*));
    MOCK_METHOD1(dbg_println, size_t(const char*));
    MOCK_METHOD3(waitForInputBlocking, ResultType(string, string*, uint32_t));
    MOCK_METHOD0(readLineToString, string());
};
