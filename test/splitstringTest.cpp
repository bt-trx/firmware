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

#include "gtest/gtest.h"

#include "../src/splitstring.h"

TEST(SplitStringTest, splitString_1Element) {
  vector<string> result = splitString("TEST1");
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(0, strcmp(result.at(0).c_str(), "TEST1"));
}

TEST(SplitStringTest, splitString_2Elements) {
  vector<string> result = splitString("TEST1 TEST2");
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(0, strcmp(result.at(0).c_str(), "TEST1"));
  ASSERT_EQ(0, strcmp(result.at(1).c_str(), "TEST2"));
}

TEST(SplitStringTest, splitString_LeadingBlank) {
  vector<string> result = splitString(" TEST1");
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(0, strcmp(result.at(0).c_str(), "TEST1"));
}

TEST(SplitStringTest, splitString_TrailingBlank) {
  vector<string> result = splitString("TEST1 ");
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(0, strcmp(result.at(0).c_str(), "TEST1"));
}

TEST(SplitStringTest, containsStringOnPosition_success_1elem) {
  ASSERT_EQ(true, containsStringOnPosition("FOO", "FOO", 0));
}

TEST(SplitStringTest, containsStringOnPosition_success_2elem) {
  ASSERT_EQ(true, containsStringOnPosition("FOO BAR", "BAR", 1));
}

TEST(SplitStringTest, containsStringOnPosition_fail_1elem) {
  ASSERT_EQ(false, containsStringOnPosition("FOO", "BAR", 0));
}

TEST(SplitStringTest, containsStringOnPosition_fail_2elem) {
  ASSERT_EQ(false, containsStringOnPosition("FOO BAR", "BAZ", 1));
}