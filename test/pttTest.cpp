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

#include "../src/ptt.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArrayArgument;
using ::testing::StrEq;

namespace
{
class PTTTest : public ::testing::Test {
    protected:
	ArduinoMock *arduinoMock;
  PTT *ptt;

	PTTTest()
	{
	}

	virtual ~PTTTest()
	{
	}

	virtual void SetUp()
	{
		arduinoMock = arduinoMockInstance();

		EXPECT_CALL(*arduinoMock, pinMode(0, OUTPUT));
    EXPECT_CALL(*arduinoMock, pinMode(1, OUTPUT));
		ptt = new PTT(0, 1);
	}

	virtual void TearDown()
	{
		releaseArduinoMock();
		delete ptt;
	}
};

TEST_F(PTTTest, on)
{
  ptt->off();
  ASSERT_EQ(false, ptt->getState());
	EXPECT_CALL(*arduinoMock, digitalWrite(0, 0)); // ptt
  EXPECT_CALL(*arduinoMock, digitalWrite(1, 1)); // led
	ptt->on();
  ASSERT_EQ(true, ptt->getState());
}

TEST_F(PTTTest, off)
{
  ptt->on();
  ASSERT_EQ(true, ptt->getState());
	EXPECT_CALL(*arduinoMock, digitalWrite(0, 1)); // ptt
  EXPECT_CALL(*arduinoMock, digitalWrite(1, 0)); // led
	ptt->off();
  ASSERT_EQ(false, ptt->getState());
}

TEST_F(PTTTest, toggle)
{
  ptt->on();
  ASSERT_EQ(true, ptt->getState());
	ptt->toggle();
  ASSERT_EQ(false, ptt->getState());
	ptt->toggle();
  ASSERT_EQ(true, ptt->getState());
}

TEST_F(PTTTest, delayed_off)
{
  ptt->on();
  EXPECT_CALL(*arduinoMock, millis())
		.WillOnce(Return(0))
		.WillOnce(Return(50))
    .WillOnce(Return(101));
	EXPECT_CALL(*arduinoMock, digitalWrite(0, 1)); // ptt
  EXPECT_CALL(*arduinoMock, digitalWrite(1, 0)); // led
	ptt->delayed_off(100);
  ASSERT_EQ(true, ptt->getState());
  ptt->delayed_off(100);
  ASSERT_EQ(true, ptt->getState());
  ptt->delayed_off(100);
  ASSERT_EQ(false, ptt->getState());
}

} // namespace
