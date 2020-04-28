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

extern Preferences preferences;

BTTRX_CONTROL::BTTRX_CONTROL(SerialWrapperInterface *_serial,
                             WT32iInterface *_wt32i)
    : serial_(_serial), wt32i_(_wt32i) {}

/**
 * @brief Evaluate SET command (e.g. from Webserver)
 *
 * @param name Parameter name as string
 * @param value Parameter value as string
 * @return ResultType
 */
ResultType BTTRX_CONTROL::set(string name, string value) {
  // Search for name in enum
  ParameterType parameter = stringToParameterType(name);
  // Call handler method
  ResultType result = kError;
  switch (parameter) {
  case kCallsign:
    result = handleSetCallsign(value);
    break;
  case kADCGain:
    result = handleSetADCGain(value);
    break;
  case kDACGain:
    result = handleSetDACGain(value);
    break;
  case kPinCode:
    result = handleSetPinCode(value);
    break;
  case kPTTMode:
    result = handleSetPTTMode(value);
    break;
  case kPTTTimeout:
    result = handleSetPTTTimeout(value);
    break;
  case kPTTHangTime:
    result = handleSetPTTHangTime(value);
    break;
  default:
    return kError;
    break;
  }
  return result;
}

/**
 * @brief Evalute GET command (e.g. from Webserver)
 *
 * @param name Parameter name as string
 * @param value output: Parameter value as string
 * @return ResultType
 */
ResultType BTTRX_CONTROL::get(string name, string *value) {
  // Search for name in enum
  ParameterType parameter = stringToParameterType(name);
  return get(parameter, value);
}

/**
 * @brief Evalute GET command (e.g. from Webserver)
 * Parameters may be stored as "Parameter" in Flash or read from WT32i
 *
 * @param name Parameter name as ParameterType
 * @param value output: Parameter value as string
 * @return ResultType
 */
ResultType BTTRX_CONTROL::get(ParameterType parameter, string *value) {
  // Reply with value / call handler method
  switch (parameter) {
  case kStatusmessage:
    *value = status_message_;
    break;
  case kCallsign:
    *value = preferences.getString(ParameterTypeToString(kCallsign).c_str(), "")
                 .c_str();
    break;
  case kADCGain:
    *value = adc_gain_;
    break;
  case kDACGain:
    *value = dac_gain_;
    break;
  case kPinCode:
    *value = pin_code_;
    break;
  case kPTTMode:
    *value = to_string(
        preferences.getUShort(ParameterTypeToString(kPTTMode).c_str(), 1));
    break;
  case kPTTTimeout:
    *value = to_string(
        preferences.getUShort(ParameterTypeToString(kPTTTimeout).c_str(), 3));
    break;
  case kPTTHangTime:
    *value = to_string(
        preferences.getUShort(ParameterTypeToString(kPTTHangTime).c_str(), 0));
    break;
  default:
    return kError;
    break;
  }
  return kSuccess;
}

ResultType BTTRX_CONTROL::get(ParameterType parameter, bool *value) {
  switch (parameter) {
  default:
    return kError;
  }
  return kSuccess;
}

/**
 * @brief Perform an action (call a method)
 *
 * @param name Action parameter as string
 * @return ResultType
 */
ResultType BTTRX_CONTROL::action(string name) {
  if (name == "resetBTPairings") {
    wt32i_->resetBTPairings();
    return kSuccess;
  }
  return kError;
}

/**
 * @brief Store current value of a parameter in member variable
 * Used during startup to store values read from WT32i to this object
 *
 * @param type
 * @param value
 */
void BTTRX_CONTROL::storeSetting(ParameterType type, string value) {
  switch (type) {
  case kADCGain:
    adc_gain_ = value;
    break;
  case kDACGain:
    dac_gain_ = value;
    break;
  case kPinCode:
    pin_code_ = value;
    break;
  case kStatusmessage:
    status_message_ = value;
    break;
  default:
    break;
  }
}

/**
 * @brief Getter method for Callsign
 *
 * @return Callsign
 */
string BTTRX_CONTROL::getCallsign() {
  string value = "";
  get(kCallsign, &value);
  return value;
}

/**
 * @brief Getter method for PTT Mode
 *
 * @return PTTMode
 */
PTTMode BTTRX_CONTROL::getPTTMode() {
  string value = "";
  get(kPTTMode, &value);
  return (PTTMode)stoi(value);
}

/**
 * @brief Getter method for PTT Timeout
 *
 * @return uint16_t
 */
uint16_t BTTRX_CONTROL::getPTTTimeout() {
  string value = "";
  get(kPTTTimeout, &value);
  return stoi(value);
}

/**
 * @brief Getter method for PTT HangTime
 *
 * @return uint16_t
 */
uint16_t BTTRX_CONTROL::getPTTHangTime() {
  string value = "";
  get(kPTTHangTime, &value);
  return stoi(value);
}

/**
 * @brief Convert parameter string to parameter Type
 *
 * @param name
 * @return ParameterType
 */
ParameterType BTTRX_CONTROL::stringToParameterType(string name) {
  if (name == "statusmessage") {
    return kStatusmessage;
  }
  if (name == "callsign") {
    return kCallsign;
  }
  if (name == "adc_gain") {
    return kADCGain;
  }
  if (name == "dac_gain") {
    return kDACGain;
  }
  if (name == "pin_code") {
    return kPinCode;
  }
  if (name == "ptt_mode") {
    return kPTTMode;
  }
  if (name == "ptt_timeout") {
    return kPTTTimeout;
  }
  if (name == "ptt_hang_time") {
    return kPTTHangTime;
  }
  return kUnkownParameter;
}

/**
 * @brief Convert Parameter Type to String
 *
 * @param parameter_type
 * @return string
 */
string BTTRX_CONTROL::ParameterTypeToString(ParameterType parameter_type) {
  string return_value = "unknown parameter";
  switch (parameter_type) {
  case kStatusmessage:
    return_value = "statusmessage";
    break;
  case kCallsign:
    return_value = "callsign";
    break;
  case kADCGain:
    return_value = "adc_gain";
    break;
  case kDACGain:
    return_value = "dac_gain";
    break;
  case kPinCode:
    return_value = "pin_code";
    break;
  case kPTTMode:
    return_value = "ptt_mode";
    break;
  case kPTTTimeout:
    return_value = "ptt_timeout";
    break;
  case kPTTHangTime:
    return_value = "ptt_hang_time";
    break;
  default:
    break;
  }
  return return_value;
}

/**
 * @brief Set Callsign
 *
 * @param callsign
 * @return ResultType
 */
ResultType BTTRX_CONTROL::handleSetCallsign(string callsign) {
  if (callsign.length() > CALLSIGN_LENGTH) {
    serial_->dbg_println("Callsign exceed maximum length. Max. length is: " +
                         CALLSIGN_LENGTH);
    return kError;
  } else {
    if (!callsign.empty()) {
      serial_->dbg_println("Set Callsign to: " + callsign);
    } else {
      serial_->dbg_println("Callsign deleted");
    }
  }

  preferences.putString(ParameterTypeToString(kCallsign).c_str(),
                        callsign.c_str());
  return kSuccess;
}

/**
 * @brief Set ADC Gain
 *
 * @param adc_gain
 * @return ResultType
 */
ResultType BTTRX_CONTROL::handleSetADCGain(string adc_gain) {
  serial_->dbg_println("Set ADC Gain to: " + adc_gain);

  // TODO Check value range

  adc_gain_ = adc_gain;
  // Set on wt32i
  return wt32i_->setAudioGain(adc_gain_, dac_gain_);
}

/**
 * @brief Set DAC Gain
 *
 * @param dac_gain
 * @return ResultType
 */
ResultType BTTRX_CONTROL::handleSetDACGain(string dac_gain) {
  serial_->dbg_println("Set DAC Gain to: " + dac_gain);

  // TODO Check value range

  dac_gain_ = dac_gain;
  // Set on wt32i
  return wt32i_->setAudioGain(adc_gain_, dac_gain_);
}

/**
 * @brief Set PIN Code
 *
 * @param pin_code
 * @return ResultType
 */
ResultType BTTRX_CONTROL::handleSetPinCode(string pin_code) {
  serial_->dbg_println("Set PIN to: " + pin_code);

  // TODO Check value range
  if (pin_code.length() != 4) {
    return kError;
  }

  pin_code_ = pin_code;
  // Set on wt32i
  return wt32i_->setPinCode(pin_code_);
}

/**
 * @brief Set PTT Toggle feature on or off
 *
 * @param enabled
 * @return ResultType
 */
ResultType BTTRX_CONTROL::handleSetPTTMode(string ptt_mode) {
  uint16_t value = stoi(ptt_mode);
  if (value < 4) {
    preferences.putUShort(ParameterTypeToString(kPTTMode).c_str(), value);
    return kSuccess;
  }
  return kError;
}

/**
 * @brief Set PTT Timeout
 *
 * @param timeout in minutes
 * @return ResultType
 */
ResultType BTTRX_CONTROL::handleSetPTTTimeout(string timeout) {
  uint16_t value = stoi(timeout);
  if (value < 10) {
    preferences.putUShort(ParameterTypeToString(kPTTTimeout).c_str(), value);
    return kSuccess;
  }
  return kError;
}

/**
 * @brief Set PTT HangTime
 *
 * @param hang_time
 * @return ResultType
 */
ResultType BTTRX_CONTROL::handleSetPTTHangTime(string hang_time) {
  uint16_t value = stoi(hang_time);
  if (value < 1000) {
    preferences.putUShort(ParameterTypeToString(kPTTHangTime).c_str(), value);
    return kSuccess;
  }
  return kError;
}
