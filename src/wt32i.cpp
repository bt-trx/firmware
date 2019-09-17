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

/*
The methods in this file are mostly based on
Silicon Labs UG218 "iWRAP 5.6.0, iWRAP 6.1.0 and iWRAP 6.1.1
USER GUIDE AND API REFERENCE", Document Revision 3.7
and
AN992: HANDS-FREE AND HEADSET PROFILES iWRAP APPLICATION NOTE, Version 1.12
*/

#include "wt32i.h"

#include "splitstring.h"


/**
 * @brief Construct a new WT32i::WT32i object
 * 
 * @param serial Serial interface for communication with WT32i module
 */
WT32i::WT32i(SerialWrapperInterface* serial)
  : serial_(serial) {
}

/**
 * @brief Sends RESET command to WT32i
 * 
 * @return ResultType Always returns kSuccess
 */
ResultType WT32i::reset() {
  serial_->println("RESET");
  return ResultType::kSuccess;
}

/**
 * @brief Check for availability of the WT32i module
 * Blocks until answer or timeout
 * 
 * @return ResultType Returns kSuccess if WT32i replied within timeout
 */
ResultType WT32i::available() {
  serial_->println("AT");

  return serial_->waitForInputBlocking("OK");
}

/**
 * @brief Set iWrap configuration values
 * 
 * see UG218 Page 118 * 
 * Example: "SET BT NAME WT12" -> set("BT", "NAME", "WT12")
 * 
 * There is no verification if the parameter has been set successfully
 * 
 * @param category 
 * @param option 
 * @param value 
 * @return ResultType 
 */
ResultType WT32i::set(string category,
                      string option,
                      string value) {
  string output = "SET ";
  if (!category.length()) {
    return ResultType::kError;
  }
  output += category;

  if (option.length() && value.length()) {
    output += " " + option;
    output += " " + value;
  } else if ((option.length() && !value.length())
             || (!option.length() && value.length())) {
    return ResultType::kError;
  }

  serial_->println(output.c_str());
  return ResultType::kSuccess;
}

ResultType WT32i::startInquiry() {
  // Clear current list of currently known devices
  inquired_devices_.clear();

  // Inquiry for x * 1.28 seconds
  string output = "INQUIRY ";
  output.append(INQUIRY_DURATION);
  output.append(" NAME");
  serial_->println(output.c_str());

  inquiry_running_ = true;

  return kSuccess;
}

/**
 * @brief Perform inquiry of bluetooth devices nearby
 * Blocks until answer or timeout
 * 
 * The list of inquiry results gets written to the inquired_devices_ list.
 *  
 * @return ResultType 
 */
ResultType WT32i::inquiry() {
  // Clear current list of currently known devices
  inquired_devices_.clear();

  // Inquiry for x * 1.28 seconds
  serial_->println("INQUIRY 5"); // 5 * 1.28 sec

  // Expecting "INQUIRY <num_devices>"
  // Increased timeout, as Inquiry 5 takes longer than the default timeout
  string output;
  if (serial_->waitForInputBlocking("INQUIRY", &output, 8000)
        == ResultType::kTimeoutError) {
    return ResultType::kTimeoutError;
  }

  // Reading inquiry data of <num_devices> and put it into inquired_devices_
  int num_results = stoi(splitString(output)[1]);
  for (int i=0; i<num_results; i++) {
    if (serial_->waitForInputBlocking("INQUIRY", &output)
          == ResultType::kTimeoutError) {
      return ResultType::kTimeoutError;
    }
    inquired_devices_.push_back(splitString(output)[1]);
  }
  return ResultType::kSuccess;
}


ResultType WT32i::readActiveConnections() {
  active_connections_.clear();
  serial_->println("LIST");
  return kSuccess;
}

/**
 * @brief Get list of active connections
 * Blocks until answer or timeout
 * 
 * The list of bluetooth addresses gets gets written
 * to the active_connections_ list.
 *  
 * @return ResultType 
 */
ResultType WT32i::list() {
  // Clear current list of currently active connections
  active_connections_.clear();

  serial_->println("LIST");

  // Expecting "LIST <number_of_connections>"
  string output;
  if (serial_->waitForInputBlocking("LIST", &output)
        == ResultType::kTimeoutError) {
    return ResultType::kTimeoutError;
  }
  int number_of_connections = stoi(splitString(output)[1]);

  //Reading data of <num_devices> and put it into active_connections_
  for (int i=0; i<number_of_connections; i++) {
    if (serial_->waitForInputBlocking("LIST", &output)
          == ResultType::kTimeoutError) {
      return ResultType::kTimeoutError;
    }
    // The bluetooth address is on position 10 of the reply
    active_connections_.push_back(splitString(output)[10]);
  }
  return ResultType::kSuccess;
}

ResultType WT32i::connectHFPAGnonblocking(string address) {
  string output = "call " + address + " 111e hfp-ag";
  serial_->println(output.c_str());
  return kSuccess;
}

/**
 * @brief Establish HFP-AG connection to given device
 * Blocks until answer or timeout
 * 
 * @param address Bluetooth address of the HFP device
 * @return ResultType 
 */
ResultType WT32i::connectHFPAG(string address) {
  string output = "call " + address + " 111e hfp-ag";
  serial_->println(output.c_str());

  string input;
  if (serial_->waitForInputBlocking("CALL", &input)
        == kTimeoutError) {
    return kTimeoutError;
  }
  
  // Setting timeout to 15 secs, in case of fallback to PIN
  // This should be somewhat enough time to enter the PIN in the HFP device
  ulong start_time = millis();
  while (true) {
    input = serial_->readLineToString();
    if (!input.empty()) {
      vector<string> splitted_input = splitString(input);
      if (splitted_input[0] == "SSP" && splitted_input[1] == "CONFIRM") {
        output = "SSP CONFIRM " + address + " OK";
        serial_->println(output.c_str());
      } else if (splitted_input[0] == "CONNECT") {
        break;
      }
    }

    if ((start_time + 15000) < millis()) {
      return kTimeoutError;
    }
  }

  input.clear();
  while (!containsStringOnPosition(input, "READY", 2)) {
    if (serial_->waitForInputBlocking("HFP-AG", &input)
        == kTimeoutError) {
      return kTimeoutError;
    }
  }
  return kSuccess;
}

/**
 * @brief Indication of network status
 * Should be called right after establishing the HFP-AG connection
 * 
 * Example: STATUS service 1
 * 
 * @param option 
 * @param value 
 * @return ResultType 
 */
ResultType WT32i::setStatus(string option, string value) {
  string output = "STATUS ";
  output += option + " " + value;
  serial_->println(output.c_str());
  return kSuccess;
}

/**
 * @brief Store information of HFP-AG status indications to hfp_states_ map
 * 
 * With an active HFP-AG connection, the HFP device can indicate status
 * information (e.g. battery level, volume setting, ...).
 *  
 * @param input String with HFP status indication, e.g. "HFP 0 STATUS "service" 0"
 * @return ResultType 
 */
ResultType WT32i::storeHFPStatus(string input) {
  vector<string> splitted_string = splitString(input);
  if (splitted_string.size() < 5) { return ResultType::kError; };
  if (splitted_string.at(0) != "HFP"
      || splitted_string.at(2) != "STATUS") { return ResultType::kError; };
  
  link_id_t link_id = stoi(splitted_string.at(1));
  string status_name = splitted_string.at(3);

  // Remove "" of property name ("service")
  status_name.erase(status_name.begin());
  status_name.erase(status_name.end()-1);
  int status_value = stoi(splitted_string.at(4));

  hfp_states_[link_id][status_name] = status_value;

  return ResultType::kSuccess;
}

/**
 * @brief Get value of a HFP status parameter from hfp_states_ map
 * 
 * @param link_id 
 * @param status_name 
 * @param status_value 
 * @return ResultType 
 */
ResultType WT32i::getHFPStatus(int link_id,
                               string status_name,
                               int* status_value) {
  hfp_status_t::iterator it;
  it = hfp_states_[link_id].find(status_name);
  if (it == hfp_states_[link_id].end()) {
    return ResultType::kError;
  }
  *status_value = it->second;
  return ResultType::kSuccess;
}

/**
 * @brief Indicate outgoing phone call to the HFP device
 * 
 * @return ResultType 
 */
ResultType WT32i::dial() {
  serial_->println("DIALING");

  string input;
  if (serial_->waitForInputBlocking("HFP-AG", &input)
        == ResultType::kTimeoutError) {
    return ResultType::kTimeoutError;
  }
  if(input != "HFP-AG 0 CALLING") {
    return ResultType::kError;
  }
  return ResultType::kSuccess;
}

/**
 * @brief Accept phone call request
 * 
 * @return ResultType 
 */
ResultType WT32i::connect() {
  serial_->println("CONNECT");

  string input;
  if (serial_->waitForInputBlocking("HFP-AG", &input)
        == ResultType::kTimeoutError) {
    return ResultType::kTimeoutError;
  }
  if(input != "HFP-AG 0 CONNECT") {
    return ResultType::kError;
  }
  return ResultType::kSuccess;
}

/**
 * @brief Terminate phone call
 * 
 * @return ResultType 
 */
ResultType WT32i::hangup() {
  serial_->println("HANGUP");

  return ResultType::kSuccess;
}

/**
 * @brief Poll a line from BT serial and dissect incoming iWrap Messages
 * 
 * If the incoming serial line matches with a known iWrap message, it is 
 * put into the appropriate iWrapMessage struct
 * 
 * @param msg Pointer to iWrapMessage struct (output)
 * @return ResultType 
 */
ResultType WT32i::getIncomingMessage(iWrapMessage* msg) {
  string input = serial_->readLineToString();
  return parseMessageString(input, msg);
}

/**
 * @brief Parse a string into a iWrapMessage struct
 * 
 * @param input A string containing a line coming from the Bluetooth module
 * @param msg Pointer to iWrapMessage struct (output)
 * @return ResultType
 */
ResultType WT32i::parseMessageString(string input, iWrapMessage* msg) {
  if(input.empty()) {
    msg->msg_type = kEmpty;
    return kSuccess;
  }

  vector<string> splitted_msg = splitString(input);

  if (splitted_msg[0] == "LIST") {
    if(splitted_msg.size() > 2) {
      active_connections_.push_back(splitted_msg[10]); // BT Address
      (*msg).msg_type = kLIST_RESULT;
      (*msg).msg = input;
    }
  } else if (splitted_msg[0] == "INQUIRY") {
    if (splitted_msg.size() == 2) {
      if (stoi(splitted_msg[1]) == 0) {
        inquiry_running_ = false;
      }
    }
    if (splitted_msg.size() > 2) {
        inquired_devices_.push_back(splitted_msg[1]); //  BT Address      
        inquiry_running_ = false;
        (*msg).msg_type = kINQUIRY_RESULT;
        (*msg).msg = input; 
      }
  } else if (splitted_msg[0] == "HFP-AG") {
    if (splitted_msg[2] == "READY") {
      (*msg).msg_type = kHFPAG_READY;
      (*msg).msg = input;
    } else if (splitted_msg[2] == "CALLING") {
      (*msg).msg_type = kHFPAG_CALLING;
      (*msg).msg = input;
    } else if (splitted_msg[2] == "NO" && splitted_msg[3] == "CARRIER") {
      (*msg).msg_type = kHFPAG_NO_CARRIER;
      (*msg).msg = input;
    } else if (splitted_msg[2] == "UNKNOWN") {
      (*msg).msg_type = kHFPAG_UNKOWN;
      (*msg).msg = input;
    } else {
      return kError;
    }
  } else if (splitted_msg[0] == "NO" && splitted_msg[1] == "CARRIER") {
    if (splitted_msg[3] == "ERROR") {
      if (splitted_msg[2] == "1") {
        (*msg).msg_type = kNOCARRIER_ERROR_CALL_ENDED;
        (*msg).msg = input;
      } else if (splitted_msg[2] == "0") {
        (*msg).msg_type = kNOCARRIER_ERROR_LINK_LOSS;
        (*msg).msg = input;
      }
    } else {
      return kError;
    }
  } else if (splitted_msg[0] == "SSP" && splitted_msg[1] == "CONFIRM") {
    string response = "SSP CONFIRM " + splitted_msg[2] + " OK";
    serial_->println(response.c_str());
  } else {
    return kError;
  }
  return kSuccess;
}

/**
 * @brief Handle incoming DIAL indication from HFP device
 * 
 * @param msg 
 * @return ResultType 
 */
ResultType WT32i::handleMessage_HFPAG_DIAL(iWrapMessage msg) {
  string input;
  if (serial_->waitForInputBlocking("CONNECT", &input)
        == ResultType::kTimeoutError) {
    return ResultType::kTimeoutError;
  }
  if(input != "CONNECT 1 SCO") {
    return ResultType::kError;
  }
  if (serial_->waitForInputBlocking("HFP-AG", &input)
        == ResultType::kTimeoutError) {
    return ResultType::kTimeoutError;
  }
  if(input != "HFP-AG 0 CALLING") {
    return ResultType::kError;
  }
  return ResultType::kSuccess;
}

/**
 * @brief Handle unrecognized AT commands from HFP device
 * 
 * @param msg 
 * @return ResultType 
 */
ResultType WT32i::handleMessage_HFPAG_UNKNOWN(iWrapMessage msg) {
  vector<string> splitted_msg = splitString(msg.msg);
  string cmd = splitted_msg[4];

  // If \r at the end of the string, remove it
  if (cmd.at(cmd.length()-2) == '\\' && cmd.at(cmd.length()-1) == 'r') {
    cmd.erase(cmd.length()-1);
    cmd.erase(cmd.length()-1);
  }

  // Not sure if we have to send "OK" on the end of each +C... answer, so this 
  // is currently a bit trial and error and will be fixed gradually

  if (cmd == "AT+NREC=0") {
    // Indicate that we do not support Error Cancelation and Noise cancelation
    serial_->println("ERROR");
  } else if (cmd == "AT+CGMR" || cmd == "AT+GMR") {
    // Return Manufacturer OS revision
    serial_->println("+CGMR: \"1.0\"");
    serial_->println("OK");
  } else if (cmd == "AT+CGSN" || cmd == "AT+GSN") {
    // Return serial number
    serial_->println("+CGSN: 123456");
    serial_->println("OK");
  } else if (cmd == "AT+CGMI" || cmd == "AT+CGMI?" || cmd == "AT+GMI" || cmd == "AT+GMI?") {
    // Return Manufacturer identification
    serial_->println("+CGMI: bt-trx");
    serial_->println("OK");
  } else if (cmd == "AT+CGMM" || cmd == "AT+CGMM?"|| cmd == "AT+GMM"|| cmd == "AT+GMM?") {
    // Return Model identification
    serial_->println("+CGMM: bt-trx");
    serial_->println("OK");
  } else if (cmd == "AT+CREG?") {
    //  Return Network registration
    serial_->println("+CREG: 1,1"); // registered in home network
    serial_->println("OK");
  } else if (cmd == "AT+CREG=?") {
    // Return Network registration
    serial_->println("+CREG: 0"); // GSM
    serial_->println("OK");
  } else if (cmd == "AT+BTRH?") {
    // Bluetooth respond and hold
    serial_->println("+BTRH: 1"); // Accept the call which was held
    serial_->println("OK");
  } else if (cmd == "AT+CPBS=?") {
    // Return available phonebook storages
    // e.g. "ME" (internal), "SM" (SIM) - we only support SIM
    serial_->println("+CPBS: \"ME\"");
    serial_->println("OK");
  } else if (cmd == "AT+CPBS?") {
    // Return status of currently chosen phonebook storage
    serial_->println("+CPBS: \"ME\", 1, 100"); // name, used entries, max entries
    serial_->println("OK");
  } else if (cmd == "AT+CPBR=?") {
    // Return phonebook configuration
    serial_->println("+CPBR: (1-10),20,18"); // entry range, max number length, max name length
    serial_->println("OK");
  } else if (cmd == "AT+CSCS?" || cmd == "AT+CSCS=?") {
    // Return supported character sets
    // possible values: "GSM", "HEX"."IRA", "PCDN", "UCS2","UTF-8" etc.
    serial_->println("+CSCS: GSM");
    serial_->println("OK");
  } else if (cmd == "AT+CMGS=?" || cmd == "AT+CMSS=?" || cmd == "AT+CMGL=?"
             || cmd == "AT+CMGR=?" || cmd == "AT+CNMI=?") {
    // Check if commands for sending, receiving and reading SMS are supported
    // -> No, we do not support this
    serial_->println("ERROR");
  } else if (cmd == "AT+CIMI" || cmd == "AT+CIMI?" || cmd == "AT+CIMI=?") {
    // Return IMSI
    serial_->println("+CIMI: 262735573557355");
    serial_->println("OK");
  } else if (cmd == "AT+CSQ" || cmd == "AT+CSQ?") {
    // Return signal quality (3GPP TS 27.007)
    serial_->println("+CSQ: 31,0"); // RSSI (table), BER
    serial_->println("OK");
  } else if (cmd == "AT+CBC" || cmd == "AT+CBC=?") {
    // Return battery status
    serial_->println("+CBC: 0,100"); // Battery connected, 100 %
    serial_->println("OK");
  } else if (cmd == "ATI" || cmd == "ATI0") {
    // Return Identification Information
    // Defined in 3GPP TS 27.007
    serial_->println("bt-trx 1.0");
    serial_->println("OK");
  } else if (cmd.find("AT+CPBR=") != string::npos
             && cmd != "AT+CPBR=?") {
    // Return phonebook contents
    // position, number, 129 (unkown number format), name
    serial_->println("+CPBR: 1,\"737373\",129,\"bt-trx\"");
    serial_->println("OK");
  } else if (cmd == "AT+APLSIRI?") {
    // Return SIRI is not available on this platform
    serial_->println("+APLSIRI:0");
    serial_->println("OK");
  }

  // Commands which are recognized, but ignored
  else if (cmd.find("AT+XAPL=") != string::npos) {
    // Indicates apple specific capabilities of the accessory
  } else if (cmd.find("AT+CMGF=") != string::npos) {
    // Set SMS Text Mode (0) or PDU Mode (1)
    serial_->println("OK");
  } else if (cmd.find("AT+CNMI=") != string::npos) {
    // Configuration of message routing/display of new messages
    serial_->println("OK");
  } else if (cmd.find("AT+CSCS=\"") != string::npos) {
    // Set charset to use
    serial_->println("OK");
  } else if (cmd.find("AT+CSRSF=\"") != string::npos) {
    // TODO Find out what this command is used for (seen in Fiat Fiorino)
    serial_->println("OK");
  } else if (cmd.find("AT+CPMS=") != string::npos) {
    // Set preferred message storage
    serial_->println("OK");
  } else if (cmd.find("AT+CPBS=\"") != string::npos) {
    // Set currently used phonebook storage
    serial_->println("OK");
  } else if (cmd == "AT+CREG=0" || cmd == "AT+CREG=1" ||cmd == "AT+CREG=2") {
    // Set behavior in case of network status change  
    serial_->println("OK");
  } else if (cmd == "ATE0") {
    // Disable echo of commands
  }

  // Unkown commands
  else {
    serial_->dbg_println("INFO: unhandled message");
    return kError;
  }

  return kSuccess;
}

/**
 * @brief Indicate the availability of the mobile phone network to the HFP device
 * 
 * @return ResultType 
 */
ResultType WT32i::indicateNetworkAvailable() {
  setStatus("service", "1");
  setStatus("signal", "5");
  return kSuccess;
}
