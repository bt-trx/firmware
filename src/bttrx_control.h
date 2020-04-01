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

#include "resulttype.h"
#include "serialwrapper.h"
#include "wt32i.h"

#ifdef ARDUINO
#include "Preferences.h"
#else
#include "../test/esp32_mock/Preferences.h"
#endif

#include <string>
using namespace std;

enum ParameterType {
  kUnkownParameter,
  kADCGain,
  kDACGain,
  kPinCode,
  kPTTToggleEnabled,
  kPTTTimeout,
  kPTTHangTime,
  kPTTWillimodeEnabled
};

class BTTRX_CONTROL {
public:
  BTTRX_CONTROL(SerialWrapperInterface *, WT32iInterface *);
  ResultType set(string, string);
  ResultType get(string, string *);
  ResultType get(ParameterType, string *);
  ResultType get(ParameterType, bool *);
  ResultType action(string);
  void storeSetting(ParameterType, string);

  bool getPTTToggleEnabled();
  uint16_t getPTTTimeout();
  uint16_t getPTTHangTime();
  bool getPTTWillimodeEnabled();

private:
  SerialWrapperInterface *serial_;
  WT32iInterface *wt32i_;

  ParameterType stringToParameterType(string);
  string ParameterTypeToString(ParameterType);
  ResultType handleSetADCGain(string);
  ResultType handleSetDACGain(string);
  ResultType handleSetPinCode(string);
  ResultType handleSetPTTToggleEnabled(string);
  ResultType handleSetPTTTimeout(string);
  ResultType handleSetPTTHangTime(string);
  ResultType handleSetPTTWillimodeEnabled(string);

  string adc_gain_ = "0";
  string dac_gain_ = "0";
  string pin_code_ = "0000";
};