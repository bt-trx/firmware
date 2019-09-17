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

#include "splitstring.h"

/**
 * @brief Splits an incoming string to a vector of string by the " " delimiter
 *
 * @param input
 * @return vector<string>
 */
vector<string> splitString(const string input)
{
	string s         = input;
	size_t pos       = 0;
	string delimiter = " ";
	vector<string> result;
	while ((pos = s.find(delimiter)) != string::npos) {
		if (pos != 0) {
			result.push_back(s.substr(0, pos));
		}
		s.erase(0, pos + delimiter.length());
	}
	if (s != delimiter && s.length()) {
		result.push_back(s);
	}
	return result;
}

/**
 * @brief Tests if the given string is on the given position of the input_string
 * Position is to be understood as "word" here, not as character.
 *
 * Example: input_string = "foo bar baz" -> position 2 is "baz"
 *
 * @param input_string
 * @param string_to_test
 * @param position
 * @return bool
 */
bool containsStringOnPosition(
	string input_string, string string_to_test, size_t position)
{
	vector<string> splitted_string = splitString(input_string);
	if (position < splitted_string.size()) {
		if (splitted_string.at(position) == string_to_test) {
			return true;
		}
	}
	return false;
}