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

#include "../src/bttrx_control.h"
#include "serialwrapperMock.h"
#include "wt32iMock.h"

using ::testing::_;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;
using ::testing::StrEq;

Preferences preferences;

namespace
{
class BTTRX_CONTROLTest : public ::testing::Test {
    protected:
	SerialWrapperMock serialWrapperMock;
	WT32iMock wt32iMock;

	BTTRX_CONTROLTest()
	{
	}

	virtual ~BTTRX_CONTROLTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};

TEST_F(BTTRX_CONTROLTest, action_resetBTPairings_success)
{
	BTTRX_CONTROL bttrx_control(&serialWrapperMock, &wt32iMock);

	EXPECT_CALL(wt32iMock, resetBTPairings());

	ASSERT_EQ(
		ResultType::kSuccess, bttrx_control.action("resetBTPairings"));
}

TEST_F(BTTRX_CONTROLTest, set_adc_gain_success)
{
	BTTRX_CONTROL bttrx_control(&serialWrapperMock, &wt32iMock);

	EXPECT_CALL(wt32iMock, setAudioGain("16", "0"))
		.WillOnce((Return(ResultType::kSuccess)));

	ASSERT_EQ(ResultType::kSuccess, bttrx_control.set("adc_gain", "16"));
}

TEST_F(BTTRX_CONTROLTest, set_dac_gain_success)
{
	BTTRX_CONTROL bttrx_control(&serialWrapperMock, &wt32iMock);

	EXPECT_CALL(wt32iMock, setAudioGain("0", "16"))
		.WillOnce((Return(ResultType::kSuccess)));

	ASSERT_EQ(ResultType::kSuccess, bttrx_control.set("dac_gain", "16"));
}

TEST_F(BTTRX_CONTROLTest, set_pin_code_success)
{
	BTTRX_CONTROL bttrx_control(&serialWrapperMock, &wt32iMock);

	EXPECT_CALL(wt32iMock, setPinCode("2342"))
		.WillOnce((Return(ResultType::kSuccess)));

	ASSERT_EQ(ResultType::kSuccess, bttrx_control.set("pin_code", "2342"));
}

TEST_F(BTTRX_CONTROLTest, set_ptt_hang_time)
{
	BTTRX_CONTROL bttrx_control(&serialWrapperMock, &wt32iMock);

	ASSERT_EQ(
		ResultType::kSuccess, bttrx_control.set("ptt_hang_time", "0"));
	ASSERT_EQ(
		ResultType::kSuccess,
		bttrx_control.set("ptt_hang_time", "999"));
	ASSERT_EQ(
		ResultType::kError, bttrx_control.set("ptt_hang_time", "1000"));
}

TEST_F(BTTRX_CONTROLTest, get_pin_code_success)
{
	BTTRX_CONTROL bttrx_control(&serialWrapperMock, &wt32iMock);

	bttrx_control.storeSetting(kPinCode, "2342");
	bttrx_control.storeSetting(kADCGain, "15");
	bttrx_control.storeSetting(kDACGain, "16");
	string output = "";

	ASSERT_EQ(ResultType::kSuccess, bttrx_control.get("pin_code", &output));
	ASSERT_EQ(0, output.compare("2342"));
	ASSERT_EQ(ResultType::kSuccess, bttrx_control.get("adc_gain", &output));
	ASSERT_EQ(0, output.compare("15"));
	ASSERT_EQ(ResultType::kSuccess, bttrx_control.get("dac_gain", &output));
	ASSERT_EQ(0, output.compare("16"));
}

} // namespace
