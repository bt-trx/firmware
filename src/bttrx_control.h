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

#include <string>
using namespace std;

enum ParameterType { kUnkownParameter, kADCGain, kDACGain, kPinCode };

class BTTRX_CONTROL {
    public:
	BTTRX_CONTROL(SerialWrapper *, WT32i *);
	ResultType set(string, string);
	ResultType get(string, string *);
	ResultType action(string);
	void storeSetting(ParameterType, string);

    private:
	SerialWrapper *serial_;
	WT32i *wt32i_;

	ParameterType getParameter(string);
	ResultType handleSetADCGain(string);
	ResultType handleSetDACGain(string);
	ResultType handleSetPinCode(string);

	string adc_gain_ = "0";
	string dac_gain_ = "0";
	string pin_code_ = "0000";
};