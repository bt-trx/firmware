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

#include "arduino-mock/Arduino.h"

#include "../src/button_hw.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArrayArgument;
using ::testing::StrEq;

namespace {
class ButtonHWTest : public ::testing::Test {
protected:
  ArduinoMock *arduinoMock;
  ButtonHW *button;

  ButtonHWTest() {}

  virtual ~ButtonHWTest() {}

  virtual void SetUp() {
    arduinoMock = arduinoMockInstance();

    EXPECT_CALL(*arduinoMock, pinMode(_, INPUT));
    button = new ButtonHW(0);
  }

  virtual void TearDown() {
    releaseArduinoMock();
    delete button;
  }
};

TEST_F(ButtonHWTest, isPressed) {
  EXPECT_CALL(*arduinoMock, digitalRead(_))
      .WillOnce(Return(0))
      .WillOnce(Return(0));
  EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(0)).WillOnce(Return(50));
  button->update();
  button->update();
  ASSERT_EQ(true, button->isPressed());
}

TEST_F(ButtonHWTest, isTripleClick_true) {
  
  button->checkForTripleClick(false, 0);
  ASSERT_EQ(false, button->isTripleClick());

  button->checkForTripleClick(true, 100);
  ASSERT_EQ(false, button->isTripleClick());
  
  button->checkForTripleClick(false,150);
  ASSERT_EQ(false, button->isTripleClick());

  button->checkForTripleClick(true, 200);
  ASSERT_EQ(false, button->isTripleClick());

  button->checkForTripleClick(false, 250);
  ASSERT_EQ(false, button->isTripleClick());

  button->checkForTripleClick(true, 300);
  ASSERT_EQ(true, button->isTripleClick());
  
  button->checkForTripleClick(false, 350);
  ASSERT_EQ(false, button->isTripleClick());
}
} // namespace
