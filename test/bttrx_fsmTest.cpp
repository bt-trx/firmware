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

#include "../src/bttrx_fsm.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::SetArrayArgument;
using ::testing::StrEq;

namespace
{
class BTTRX_FSMTest : public ::testing::Test {
    protected:
	ArduinoMock *arduinoMock;
	SerialMock *serialMock;

	BTTRX_FSMTest()
	{
	}

	virtual ~BTTRX_FSMTest()
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

TEST_F(BTTRX_FSMTest, Run_StateInit_Success1)
{
	EXPECT_CALL(*arduinoMock, pinMode(_, _)).Times(4);
	BTTRX_FSM bttrx_fsm(&Serial, &Serial);

	ASSERT_EQ(BTTRX_FSM::state_t::STATE_INIT, bttrx_fsm.getCurrentState());
}
} // namespace