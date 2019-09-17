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
#include "arduino-mock/Serial.h"

#include "../src/serialwrapper.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArrayArgument;
using ::testing::StrEq;

namespace
{
class SerialWrapperTest : public ::testing::Test {
    protected:
	ArduinoMock *arduinoMock;
	SerialMock *serialMock;

	SerialWrapperTest()
	{
	}

	virtual ~SerialWrapperTest()
	{
	}

	virtual void SetUp()
	{
		arduinoMock = arduinoMockInstance();
		serialMock  = serialMockInstance();
	}

	virtual void TearDown()
	{
		releaseSerialMock();
		releaseArduinoMock();
	}
};

TEST_F(SerialWrapperTest, waitForInputBlocking_success_shortAnswer)
{
	SerialWrapper serialwrapper = SerialWrapper(&Serial);

	EXPECT_CALL(*arduinoMock, millis())
		.WillOnce(Return(0))
		.WillOnce(Return(1));
	char output[] = "FOO";
	EXPECT_CALL(*serialMock, readBytesUntil(_, _, _))
		.WillOnce(DoAll(
			SetArrayArgument<1>(output, output + 4), (Return(3))));

	ASSERT_EQ(
		ResultType::kSuccess,
		serialwrapper.waitForInputBlocking("FOO"));
}

TEST_F(SerialWrapperTest, waitForInputBlocking_success_longAnswer)
{
	SerialWrapper serialwrapper = SerialWrapper(&Serial);

	EXPECT_CALL(*arduinoMock, millis())
		.WillOnce(Return(0))
		.WillOnce(Return(1));
	char output[] = "FOO BAR";
	EXPECT_CALL(*serialMock, readBytesUntil(_, _, _))
		.WillOnce(DoAll(
			SetArrayArgument<1>(output, output + 8), (Return(7))));

	string result;
	ASSERT_EQ(
		ResultType::kSuccess,
		serialwrapper.waitForInputBlocking("FOO", &result));
	ASSERT_EQ(0, result.compare(output));
}

TEST_F(SerialWrapperTest, waitForInputBlocking_timeout)
{
	SerialWrapper serialwrapper = SerialWrapper(&Serial);

	EXPECT_CALL(*arduinoMock, millis())
		.WillOnce(Return(0))
		.WillOnce(Return(1))
		.WillOnce(Return(BT_SERIAL_TIMEOUT));

	char output[] = "BAR";
	EXPECT_CALL(*serialMock, readBytesUntil(_, _, _))
		.WillRepeatedly(DoAll(
			SetArrayArgument<1>(output, output + 4), (Return(3))));

	ASSERT_EQ(
		ResultType::kTimeoutError,
		serialwrapper.waitForInputBlocking("FOO"));
}

TEST_F(SerialWrapperTest, readLineToString_success)
{
	SerialWrapper serialwrapper_ = SerialWrapper(&Serial);

	char output[] = "FOO BAR";
	EXPECT_CALL(*serialMock, readBytesUntil(_, _, _))
		.WillOnce(DoAll(
			SetArrayArgument<1>(output, output + 8), (Return(7))));

	ASSERT_EQ("FOO BAR", serialwrapper_.readLineToString());
}

TEST_F(SerialWrapperTest, readLineToString_success_emptyString)
{
	SerialWrapper serialwrapper_ = SerialWrapper(&Serial);

	EXPECT_CALL(*serialMock, readBytesUntil(_, _, _));

	ASSERT_EQ("", serialwrapper_.readLineToString());
}
} // namespace