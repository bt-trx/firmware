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

#ifdef ARDUINO
#include <Arduino.h>
#else
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"
#endif

#include "resulttype.h"
#include "settings.h"

#include <string>
using namespace std;

class SerialWrapperInterface {
public:
  virtual ~SerialWrapperInterface() {}
  virtual size_t println(const char *) = 0;
  virtual size_t println(string) = 0;
  virtual size_t dbg_println(const char *) = 0;
  virtual size_t dbg_println(string) = 0;
  virtual ResultType waitForInputBlocking(string, string * = NULL,
                                          uint32_t = BT_SERIAL_TIMEOUT) = 0;
  virtual string readLineToString() = 0;
};

class SerialWrapper : public SerialWrapperInterface {
public:
  SerialWrapper(){};
  SerialWrapper(Stream *, Stream * = NULL);
  ~SerialWrapper(){};

  void setSerialStreams(Stream *, Stream *);

  size_t println(const char *);
  size_t println(string);
  size_t dbg_println(const char *);
  size_t dbg_println(string);

  ResultType waitForInputBlocking(string expectation, string *output = NULL,
                                  uint32_t timeout = BT_SERIAL_TIMEOUT);
  string readLineToString();
  void pipeDbgToBtStream();

private:
  Stream *serial_bt_ = NULL;
  Stream *serial_dbg_ = NULL;
};
