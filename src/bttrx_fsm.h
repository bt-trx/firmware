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
#include "arduino-mock/Serial.h"
#endif

#include "bddeviceinfo.h"
#include "bttrx_control.h"
#include "button_ble.h"
#include "button_hw.h"
#include "led.h"
#include "ptt.h"
#include "settings.h"
#include "wt32i.h"

#include <string>
using namespace std;

class BTTRX_FSM {
public:
  enum state_t {
    STATE_INIT,
    STATE_CONFIGURE,
    STATE_INQUIRY,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_CALL_RUNNING
  };

  BTTRX_FSM();
  BTTRX_FSM(Stream *serial_bt, Stream *serial_dbg = NULL);
  void setSerial(Stream *serial_bt, Stream *serial_dbg = NULL);
  void run();

  ButtonBLE *getBLEButtonHandler() { return &ble_button_; };

  BTTRX_CONTROL bttrx_control_;

  // only required for unit testing
  state_t getCurrentState() { return current_state_; };

private:
  SerialWrapper serial_;
  WT32i wt32i_;

  state_t current_state_;
  void setState(state_t);

  LED led_connected_;
  LED led_busy_;
  ButtonHW helper_button_;
  ButtonHW ptt_button_;
  ButtonBLE ble_button_;
  PTT ptt_output_;

  vector<BDDeviceInfo> remote_devices_;
  BDDeviceInfo remote_device_info_;
  void assignBDFriendlyNameForRemoteDevice();

  // FSM State handler
  void handleStateInit();
  void handleStateConfigure();
  void handleStateInquiry();
  void handleStateConnecting();
  void handleStateConnected();
  void handleStateCallRunning();
  // Message handler
  void handleIncomingMessage();
  // Handle PTT during Call
  void handlePTTDuringCall();
  void handlePTTDirect();
  void handlePTTWiredToggle();
  void handlePTTBLEToggle();
  void handlePTTWiredWillimode();
};
