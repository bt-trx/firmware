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

#include "bttrx_control.h"

BTTRX_CONTROL::BTTRX_CONTROL(SerialWrapper* _serial, WT32i* _wt32i)
: serial_(_serial), wt32i_(_wt32i)
{}

ResultType BTTRX_CONTROL::set(string name, string value)
{
	// Search for name in enum
	ParameterType parameter = getParameter(name);
	// Call handler method
	ResultType result = kError;
	switch (parameter) {
		case kADCGain:
			result = handleSetADCGain(value);
			break;
		case kDACGain:
			result = handleSetDACGain(value);
			break;
		default:
			return kError;
			break;
	}
	return result;
}

void BTTRX_CONTROL::storeSetting(ParameterType type, string value)
{
	switch (type)
	{
		case kADCGain:
			adc_gain_ = value;
		break;
		case kDACGain:
			dac_gain_ = value;
		break;
		default:
		break;
	}
}

ParameterType BTTRX_CONTROL::getParameter(string name)
{
	if (name == "adc_gain") { return kADCGain; }
	if (name == "dac_gain") { return kDACGain; }	
	return kUnkownParameter;
}

ResultType BTTRX_CONTROL::handleSetADCGain(string adc_gain)
{
	serial_->dbg_println("Set ADC Gain to: " + adc_gain);

	// TODO Check value range

	adc_gain_ = adc_gain;
	// Set on wt32i
	return wt32i_->setAudioGain(adc_gain_, dac_gain_);
}

ResultType BTTRX_CONTROL::handleSetDACGain(string dac_gain)
{
	serial_->dbg_println("Set DAC Gain to: " + dac_gain);

	// TODO Check value range

	dac_gain_ = dac_gain;
	// Set on wt32i
	return wt32i_->setAudioGain(adc_gain_, dac_gain_);
}