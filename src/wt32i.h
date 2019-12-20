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

#pragma once

#include "settings.h"
#include "resulttype.h"
#include "iwrapmessage.h"
#include "serialwrapper.h"

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;

typedef int link_id_t;

class WT32iInterface {
    public:
	virtual void resetBTPairings()                  = 0;
	virtual ResultType setAudioGain(string, string) = 0;
	virtual ResultType setPinCode(string)           = 0;
};

class WT32i : public WT32iInterface {
    public:
	WT32i(){};
	WT32i(SerialWrapperInterface *);
	void setSerialWrapper(SerialWrapperInterface *serial)
	{
		serial_ = serial;
	};

	// Communication with WT32i device
	ResultType reset();
	ResultType available();
	void set();
	ResultType set(string, string = "", string = "");
	ResultType setAudioGain(string, string);
	ResultType setPinCode(string);
	ResultType performInquiry();
	ResultType startInquiry();
	bool inquiryRunning()
	{
		return inquiry_running_;
	}
	ResultType list();
	ResultType readActiveConnections();
	void resetBTPairings();
	ResultType connectHFPAG(string);
	ResultType connectHFPAGnonblocking(string);
	ResultType setStatus(string, string);
	ResultType dial();
	ResultType connect();
	ResultType hangup();
	ResultType scoOpen();
	ResultType scoClose();
	ResultType sendSSPConfirmation(string);

	// Message handlers
	ResultType getIncomingMessage(iWrapMessage *);
	ResultType handleMessage_HFPAG_DIAL(iWrapMessage);
	ResultType handleMessage_HFPAG_UNKNOWN(iWrapMessage);

	// Helper Methods
	ResultType storeHFPStatus(string);
	ResultType getHFPStatus(int, string, int *);
	vector<string> getInquiredDevices()
	{
		return inquired_devices_;
	};
	vector<string> getActiveConnections()
	{
		return active_connections_;
	}
	string getBDAddressSuffix();

	ResultType indicateNetworkAvailable();

	// Only public for unit testing
	ResultType parseMessageString(string, iWrapMessage *);

    private:
	typedef std::map<string, int> hfp_status_t;

	SerialWrapperInterface *serial_ = NULL;
	vector<string> inquired_devices_;
	vector<string> active_connections_;
	std::map<link_id_t, hfp_status_t> hfp_states_;

	bool inquiry_running_ = false;

	ResultType getBDAddress(string *);
	string stripBDAddress(string);

	void sendOK();
	void sendERROR();
};