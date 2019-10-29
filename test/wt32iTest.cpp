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

#include "../src/wt32i.h"
#include "serialwrapperMock.h"

using ::testing::_;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;
using ::testing::StrEq;

namespace
{
class WT32iTest : public ::testing::Test {
    protected:
	SerialWrapperMock serialWrapperMock;
	ArduinoMock *arduinoMock;

	WT32iTest()
	{
	}

	virtual ~WT32iTest()
	{
	}

	virtual void SetUp()
	{
		arduinoMock = arduinoMockInstance();
	}

	virtual void TearDown()
	{
		releaseArduinoMock();
	}
};

TEST_F(WT32iTest, reset_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("RESET"))));

	wt32i.reset();
}

TEST_F(WT32iTest, set_success_category)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("SET TEST"))));

	wt32i.set(string("TEST"));
}

TEST_F(WT32iTest, set_success_optionAndValue)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("SET TEST 123 456"))));

	wt32i.set(string("TEST"), string("123"), string("456"));
}

TEST_F(WT32iTest, set_fail_emptyCategory)
{
	WT32i wt32i(&serialWrapperMock);

	ASSERT_EQ(ResultType::kError, wt32i.set(string("")));
}

TEST_F(WT32iTest, set_success_valuenotset)
{
	WT32i wt32i(&serialWrapperMock);

	ASSERT_EQ(
		ResultType::kSuccess, wt32i.set(string("TEST"), string("123")));
}

TEST_F(WT32iTest, set_fail_optionNotSet)
{
	WT32i wt32i(&serialWrapperMock);

	ASSERT_EQ(
		ResultType::kError,
		wt32i.set(string("TEST"), string(""), string("456")));
}

TEST_F(WT32iTest, available_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock, println(Matcher<const char *>(StrEq("AT"))));

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking("OK", NULL, _))
		.WillOnce((Return(ResultType::kSuccess)));

	ASSERT_EQ(ResultType::kSuccess, wt32i.available());
}

TEST_F(WT32iTest, inquiry_success_1result)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("INQUIRY 5"))));

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking("INQUIRY", _, _))
		.WillOnce(
			DoAll(SetArgPointee<1>("INQUIRY 1"),
			      Return(ResultType::kSuccess)))
		.WillOnce(DoAll(
			SetArgPointee<1>("INQUIRY de:ad:be:ef:ca:fe 240404"),
			Return(ResultType::kSuccess)));

	ASSERT_EQ(ResultType::kSuccess, wt32i.performInquiry());

	vector<string> result = wt32i.getInquiredDevices();
	ASSERT_EQ(1, result.size());
	ASSERT_EQ(0, result[0].compare("de:ad:be:ef:ca:fe"));
}

TEST_F(WT32iTest, list_success_1result)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("LIST"))));

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking("LIST", _, _))
		.WillOnce(
			DoAll(SetArgPointee<1>("LIST 1"),
			      Return(ResultType::kSuccess)))
		.WillOnce(DoAll(
			SetArgPointee<1>(
				"LIST 0 CONNECTED HFP-AG 667 0 0 7 8d 8d de:ad:be:ef:ca:fe 3 INCOMING ACTIVE SLAVE ENCRYPTED 0"),
			Return(ResultType::kSuccess)));

	ASSERT_EQ(ResultType::kSuccess, wt32i.list());

	vector<string> result = wt32i.getActiveConnections();
	ASSERT_EQ(1, result.size());
	ASSERT_EQ(0, result[0].compare("de:ad:be:ef:ca:fe"));
}

TEST_F(WT32iTest, connectHFPAG_success_without_SSP)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(
			StrEq("call de:ad:be:ef:ca:fe 111e hfp-ag"))));

	EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(0));

	EXPECT_CALL(serialWrapperMock, readLineToString())
		.WillOnce(Return(string("CONNECT 0 HFP 3")));

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking(_, _, _))
		.WillOnce(
			DoAll(SetArgPointee<1>("CALL 0"),
			      Return(ResultType::kSuccess)))
		.WillOnce(
			DoAll(SetArgPointee<1>("HFP 0 STATUS \"service\" 0"),
			      Return(ResultType::kSuccess)))
		.WillOnce(
			DoAll(SetArgPointee<1>("HFP 0 READY"),
			      Return(ResultType::kSuccess)));

	ASSERT_EQ(
		ResultType::kSuccess, wt32i.connectHFPAG("de:ad:be:ef:ca:fe"));
}

TEST_F(WT32iTest, connectHFPAG_success_with_SSP)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(
			StrEq("call de:ad:be:ef:ca:fe 111e hfp-ag"))));
	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(
			StrEq("SSP CONFIRM de:ad:be:ef:ca:fe OK"))));

	EXPECT_CALL(*arduinoMock, millis())
		.WillOnce(Return(0))
		.WillOnce(Return(1));

	EXPECT_CALL(serialWrapperMock, readLineToString())
		.WillOnce(Return(
			string("SSP CONFIRM de:ad:be:ef:ca:fe 123456 ?")))
		.WillOnce(Return(string("CONNECT 0 HFP 3")));

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking(_, _, _))
		.WillOnce(
			DoAll(SetArgPointee<1>("CALL 0"),
			      Return(ResultType::kSuccess)))
		.WillOnce(
			DoAll(SetArgPointee<1>("HFP 0 STATUS \"service\" 0"),
			      Return(ResultType::kSuccess)))
		.WillOnce(
			DoAll(SetArgPointee<1>("HFP 0 READY"),
			      Return(ResultType::kSuccess)));

	ASSERT_EQ(
		ResultType::kSuccess, wt32i.connectHFPAG("de:ad:be:ef:ca:fe"));
}

TEST_F(WT32iTest, setStatus_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("STATUS service 1"))));

	ASSERT_EQ(ResultType::kSuccess, wt32i.setStatus("service", "1"));
}

TEST_F(WT32iTest, storeHFPStatus_success)
{
	WT32i wt32i(&serialWrapperMock);

	ASSERT_EQ(
		ResultType::kSuccess,
		wt32i.storeHFPStatus("HFP 0 STATUS \"service\" 1"));

	int value = -1;
	ASSERT_EQ(
		ResultType::kSuccess, wt32i.getHFPStatus(0, "service", &value));
	ASSERT_EQ(1, value);
}

TEST_F(WT32iTest, storeHFPStatus_fail_insert_no_status)
{
	WT32i wt32i(&serialWrapperMock);

	ASSERT_EQ(
		ResultType::kError,
		wt32i.storeHFPStatus("HFP 0 FOO \"service\" 1"));
}

TEST_F(WT32iTest, storeHFPStatus_fail_insert_too_short)
{
	WT32i wt32i(&serialWrapperMock);

	ASSERT_EQ(
		ResultType::kError,
		wt32i.storeHFPStatus("HFP 0 FOO \"service\""));
}

TEST_F(WT32iTest, getHFPStatus_fail_read)
{
	WT32i wt32i(&serialWrapperMock);

	ASSERT_EQ(
		ResultType::kSuccess,
		wt32i.storeHFPStatus("HFP 0 STATUS \"service\" 1"));

	int value = -1;
	ASSERT_EQ(ResultType::kError, wt32i.getHFPStatus(0, "foo", &value));
	ASSERT_EQ(-1, value);
}

TEST_F(WT32iTest, dial_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("DIALING"))));

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking(_, _, _))
		.WillOnce(
			DoAll(SetArgPointee<1>("HFP-AG 0 CALLING"),
			      Return(ResultType::kSuccess)));

	ASSERT_EQ(ResultType::kSuccess, wt32i.dial());
}

TEST_F(WT32iTest, connect_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("CONNECT"))));

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking(_, _, _))
		.WillOnce(
			DoAll(SetArgPointee<1>("HFP-AG 0 CONNECT"),
			      Return(ResultType::kSuccess)));

	ASSERT_EQ(ResultType::kSuccess, wt32i.connect());
}

TEST_F(WT32iTest, hangup_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("HANGUP"))));

	ASSERT_EQ(ResultType::kSuccess, wt32i.hangup());
}

TEST_F(WT32iTest, sendSSPConfirmation_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(
			StrEq("SSP CONFIRM de:ad:be:ef:23:42 OK"))));

	ASSERT_EQ(
		ResultType::kSuccess,
		wt32i.sendSSPConfirmation("de:ad:be:ef:23:42"));
}

TEST_F(WT32iTest, getIncomingMessage_success_empty)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(serialWrapperMock, readLineToString()).WillOnce(Return(""));

	iWrapMessage msg;

	ASSERT_EQ(ResultType::kSuccess, wt32i.getIncomingMessage(&msg));
	ASSERT_EQ(iWrapMessageType::kEmpty, msg.msg_type);
}

TEST_F(WT32iTest, getIncomingMessage_success_HFPAG_CALLING)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(serialWrapperMock, readLineToString())
		.WillOnce(Return("HFP-AG 0 CALLING"));

	iWrapMessage msg;

	ASSERT_EQ(ResultType::kSuccess, wt32i.getIncomingMessage(&msg));
	ASSERT_EQ(iWrapMessageType::kHFPAG_CALLING, msg.msg_type);
	ASSERT_EQ("HFP-AG 0 CALLING", msg.msg);
}

TEST_F(WT32iTest, handleMessage_HFPAG_DIAL_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking(_, _, _))
		.WillOnce(
			DoAll(SetArgPointee<1>("CONNECT 1 SCO"),
			      Return(ResultType::kSuccess)))
		.WillOnce(
			DoAll(SetArgPointee<1>("HFP-AG 0 CALLING"),
			      Return(ResultType::kSuccess)));

	iWrapMessage msg;
	msg.msg_type = kHFPAG_DIAL;
	msg.msg      = "HFP-AG 0 DIAL NUM +49123456789";
	ASSERT_EQ(ResultType::kSuccess, wt32i.handleMessage_HFPAG_DIAL(msg));
}

TEST_F(WT32iTest, handleMessage_HFPAG_UNKNOWN_NREC_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("ERROR"))));

	iWrapMessage msg;
	msg.msg_type = kHFPAG_UNKOWN;
	msg.msg      = "HFP-AG 0 UNKNOWN (0): AT+NREC=0\\r";
	ASSERT_EQ(ResultType::kSuccess, wt32i.handleMessage_HFPAG_UNKNOWN(msg));
}

TEST_F(WT32iTest, getBDAddressSuffix_success)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("SET BT BDADDR"))));

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking(_, _, _))
		.WillOnce(DoAll(
			SetArgPointee<1>("SET BT BDADDR 12:34:56:78:90:11"),
			Return(ResultType::kSuccess)));

	ASSERT_EQ(0, wt32i.getBDAddressSuffix().compare("789011"));
}

TEST_F(WT32iTest, getBDAddressSuffix_fail)
{
	WT32i wt32i(&serialWrapperMock);

	EXPECT_CALL(
		serialWrapperMock,
		println(Matcher<const char *>(StrEq("SET BT BDADDR"))));

	EXPECT_CALL(serialWrapperMock, waitForInputBlocking(_, _, _))
		.WillOnce(
			DoAll(SetArgPointee<1>(""),
			      Return(ResultType::kTimeoutError)));

	ASSERT_EQ(0, wt32i.getBDAddressSuffix().compare("1"));
}
} // namespace
