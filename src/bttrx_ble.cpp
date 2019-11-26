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

#include "bttrx_ble.h"

#include "Arduino.h"
#include "settings.h"

extern BTTRX_BLE bttrx_ble;

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
	/**
   * Called for each advertising BLE server.
   */
	void onResult(BLEAdvertisedDevice advertisedDevice)
	{
		if (!advertisedDevice.getAddress().toString().compare(
			    0, 8, BD_ADDR_OUI_ANYTONE)) {
			BLEDevice::getScan()->stop();
			bttrx_ble.doConnect(&advertisedDevice);
		}
	}
};

class MyClientCallback : public BLEClientCallbacks {
	void onConnect(BLEClient *pclient)
	{
	}

	void onDisconnect(BLEClient *pclient)
	{
		bttrx_ble.setConnectionState(false);
		SERIAL_DBG.println("BLE disconnect");
	}
};

void BTTRX_BLE::notifyCallback(
	BLERemoteCharacteristic *pBLERemoteCharacteristic,
	uint8_t *pData,
	size_t length,
	bool isNotify)
{
	SERIAL_DBG.print("BLE data: ");
	SERIAL_DBG.println((char *)pData);
}

BTTRX_BLE::BTTRX_BLE()
	: is_started(false), do_scan(true), do_connect(false),
	  is_connected(false)
{
}

void BTTRX_BLE::setupBLE()
{
	BLEDevice::init("");
	// Retrieve a Scanner and set the callback we want to use to be informed when we
	// have detected a new device.
	BLEScan *pBLEScan = BLEDevice::getScan();
	pBLEScan->setAdvertisedDeviceCallbacks(
		new MyAdvertisedDeviceCallbacks());
	SERIAL_DBG.println("BLE: setup done");
	is_started = true;
}

void BTTRX_BLE::run()
{
	if (!is_started) {
		return;
	}

	if (do_connect) {
		if (connectToDevice()) {
			SERIAL_DBG.println("BLE: Connected to device!");
		} else {
			SERIAL_DBG.println("BLE: Connection failed");
		}
		do_connect = false;
	} else if (!is_connected && do_scan) {
		// No connection, scan for BLE devices
		static ulong lastscan = 0;
		if (lastscan + (BLE_SCAN_INTERVAL * 1000) < millis()) {
			// start(duration, is_continue) is blocking,
			// start(duration, callback, is_continue) is non-blocking!
			BLEDevice::getScan()->start(
				BLE_SCAN_DURATION, nullptr, false);
			lastscan = millis();
		}
	}
}

bool BTTRX_BLE::connectToDevice()
{
	SERIAL_DBG.print("BLE: Connecting to ");
	SERIAL_DBG.println(ble_device->getAddress().toString().c_str());

	BLEClient *pClient = BLEDevice::createClient();
	pClient->setClientCallbacks(new MyClientCallback());
	pClient->connect(ble_device);

	// Obtain a reference to the service we are after in the remote BLE server.
	BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
	if (pRemoteService == nullptr) {
		SERIAL_DBG.print("BLE: Failed to find our service UUID");
		pClient->disconnect();
		return false;
	}

	// Obtain a reference to the characteristic in the service of the remote BLE server.
	BLERemoteCharacteristic *pRemoteCharacteristic =
		pRemoteService->getCharacteristic(charUUID);
	if (pRemoteCharacteristic == nullptr) {
		SERIAL_DBG.print("BLE: Failed to find our characteristic UUID");
		pClient->disconnect();
		return false;
	}

	// Subscribe to the notifications for this characteristic
	if (!pRemoteCharacteristic->canNotify()) {
		SERIAL_DBG.print("BLE: Cant register notifications");
		return false;
	}

	pRemoteCharacteristic->registerForNotify(notifyCallback);
	is_connected = true;
	return true;
}
