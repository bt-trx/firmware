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
class WT32i_parseMessageString_Test : public ::testing::Test {
    protected:
	SerialWrapperMock serialWrapperMock;

	WT32i_parseMessageString_Test()
	{
	}

	virtual ~WT32i_parseMessageString_Test()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};

TEST_F(WT32i_parseMessageString_Test, parseMessageString_success_empty)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kEmpty, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test,
       parseMessageString_success_SET_CONTROL_GAIN)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "SET CONTROL GAIN 8 10";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kSETTING_CONTROL_GAIN, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test,
       parseMessageString_success_SET_BT_AUTH)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "SET BT AUTH * 2342";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kSETTING_PIN_CODE, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test, parseMessageString_success_SET_UNKOWN)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "SET DEADBEEF";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kSETTING_UNKNOWN, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test, parseMessageString_success_LIST)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input =
		"LIST 0 CONNECTED HFP-AG 667 0 0 7 8d 8d 25:aa:92:1f:94:a8 3 INCOMING ACTIVE SLAVE ENCRYPTED 0";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kLIST_RESULT, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test, parseMessageString_success_INQUIRY)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "INQUIRY 25:aa:92:1f:94:a8 340404";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kINQUIRY_RESULT, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test, parseMessageString_success_HFPAG_CALLING)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "HFP-AG 0 CALLING";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kHFPAG_CALLING, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test,
       parseMessageString_success_HFPAG_NO_CARRIER)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "HFP-AG 0 NO CARRIER";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kHFPAG_NO_CARRIER, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test, parseMessageString_success_HFPAG_UNKOWN)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "HFP-AG 0 UNKNOWN (0): AT+NREC=0\r";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kHFPAG_UNKOWN, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test,
       parseMessageString_success_NO_CARRIER_1_ERROR_0)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "NO CARRIER 1 ERROR 0";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kNOCARRIER_ERROR_CALL_ENDED, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test,
       parseMessageString_success_NO_CARRIER_0_ERROR_L2CAP_LINK_LOSS)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "NO CARRIER 0 ERROR c0c RFC_L2CAP_LINK_LOSS";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kNOCARRIER_ERROR_LINK_LOSS, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test,
       parseMessageString_success_NO_CARRIER_0_ERROR_0)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "NO CARRIER 0 ERROR 0";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kNOCARRIER_ERROR_LINK_LOSS, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}

TEST_F(WT32i_parseMessageString_Test, parseMessageString_success_SSP_CONFIRM)
{
	WT32i wt32i(nullptr);
	iWrapMessage msg;
	string input = "SSP CONFIRM 25:aa:92:1f:94:a8 559396 ?";

	ASSERT_EQ(ResultType::kSuccess, wt32i.parseMessageString(input, &msg));
	ASSERT_EQ(iWrapMessageType::kSSP_CONFIRM, msg.msg_type);
	ASSERT_EQ(input, msg.msg);
}
} // namespace
