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

#ifdef ARDUINO

#include "BLEDevice.h"
#include "button_ble.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

static const char *BD_ADDR_OUI_ANYTONE =
	"00:1b:10"; // Anytone Bluetooth PTT BP-01
// The remote service we wish to connect to.
static BLEUUID serviceUUID("ff01"); // Button
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("ff02");

class BTTRX_BLE {
    public:
	BTTRX_BLE();
	void setupBLE(ButtonBLE *);
	ButtonBLE *getButton()
	{
		return ble_button_;
	};
	void run();
	void doConnect(BLEAdvertisedDevice *device)
	{
		ble_device = new BLEAdvertisedDevice(*device);
		do_connect = true;
	}
	void setConnectionState(bool state)
	{
		is_connected = state;
	}
	bool getConnectionState()
	{
		return is_connected;
	}

	static void
	notifyCallback(BLERemoteCharacteristic *, uint8_t *, size_t, bool);

    private:
	BLEAdvertisedDevice *ble_device;
	ButtonBLE *ble_button_ = nullptr;
	bool is_started;
	bool do_scan;
	bool do_connect;
	bool is_connected;

	bool connectToDevice();
};

#endif // ARDUINO