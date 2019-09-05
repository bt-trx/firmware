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

#include "serialwrapper.h"
#include "splitstring.h"
#include <vector>


SerialWrapper::SerialWrapper(Stream* serial_bt, Stream* serial_dbg)
     : serial_bt_(serial_bt), serial_dbg_(serial_dbg) {};

void SerialWrapper::setSerialStreams(Stream* serial_bt, Stream* serial_dbg) {
  serial_bt_ = serial_bt;
  serial_dbg_ = serial_dbg;
}

/**
 * @brief Passing through the println command to the wrapped Stream.
 * Printing also to debug Stream, if defined.
 * 
 * @param _string String to write to the Stream
 * @return Bytes written to the Stream
 */
size_t SerialWrapper::println(const char* _string) {
  if (serial_dbg_ != NULL) {
    string dbg_output = "> ";
    dbg_output += string(_string);
    serial_dbg_->println(dbg_output.c_str());
  }
  return serial_bt_->println(_string);
}

/**
 * @brief Prints to the debug Stream
 * 
 * @param _string 
 * @return size_t 
 */
size_t SerialWrapper::dbg_println(const char* _string) {
  return serial_dbg_->println(_string);
}

/**
 * @brief Reading lines of the Stream until the first word of a line 
 * matches the given expectation.
 * 
 * Example: Expectation "foo" matches line "foo bar", but does match "foobar".
 * In case of a match, the whole line is written to the output parameter.
 * 
 * @param expectation Expected word at the beginning of the read line
 * @param output Output parameter to write the matching line to
 * @param timeout After this time (ms), this method exits with kTimeoutError
 * @return Result of this operation
 */
ResultType SerialWrapper::waitForInputBlocking(string expectation,
                                               string* output,
                                               uint32_t timeout) {
  ulong start_time = millis();

  while (millis() < (start_time + timeout)) {
    string input = readLineToString();
    if (!input.empty()) {
      string first_element = splitString(input)[0];
    
      if (first_element.compare(expectation) == 0) {
        if (output != NULL) {
          *output = input;
        }
        return ResultType::kSuccess;
      }
    }
  }
  return ResultType::kTimeoutError;
}

/**
 * @brief Reading a line from the Stream. A line is defined with the 
 * SERIAL_DELIMITER line ending. Maximum SERIAL_MAX_LINE_LENGTH chars are read.
 * If defined, the line is output to the debug Stream.
 * 
 * @return Line as a string
 */
string SerialWrapper::readLineToString() {
  string output = "";
  char buffer[SERIAL_MAX_LINE_LENGTH+1] = "";  
  if (serial_bt_->readBytesUntil(SERIAL_DELIMITER,
                                  buffer,
                                  SERIAL_MAX_LINE_LENGTH)) {
    output = string(buffer);
    
    // Removing CR
    if (output.at(output.length()-1) == 13) {
      output.erase(output.length()-1);
    }

    if (serial_dbg_ != NULL) {
      serial_dbg_->println(string("< " + output).c_str());
    }
  }

  return output;
}
