# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0-rc2] - 2020-05-03

### Added

- Show connection status in the Webinterface (#44)

### Changed

- Set custom network operator name (#41)

## [1.1.0-rc1] - 2020-04-21

### Added

- BT and WLAN identification using Callsign (Thx, phl0!)
- PTT-Willimode (#36)

### Changed

- Switched to LLVM coding styleguide

## [1.0.1] - 2020-02-23

### Added

- Hardware identification for dev-board v5.0

## [1.0.0] - 2020-02-20

### Added

- PTT Toggle feature (Press the button once to assert PTT,
  press again to release PTT). Can be enabled in webinterface.
- PTT Timeout feature (maximum time of transmit up to 9 minutes).
  Can be configured in webinterface.

### Changed

- Fix AT commands to enable compatibility to Audi Q5 (and probably others)
- Doing a full reconnect of existing BT connections on power-cycle of bt-trx, should solve 
  connectivity issues with some headsets.

(changes of [1.0.0-rc1] and [0.3.1-rc1])

## [1.0.0-rc1] - 2020-02-12

### Changed

- Doing a full reconnect of existing BT connections on power-cycle of bt-trx, should solve 
  connectivity issues with some headsets.

## [0.3.1-rc1] - 2020-01-18

### Added

- PTT Toggle feature (Press the button once to assert PTT,
  press again to release PTT). Can be enabled in webinterface.
- PTT Timeout feature (maximum time of transmit up to 9 minutes).
  Can be configured in webinterface.

### Changed

- Fix AT commands to enable compatibility to Audi Q5 (and probably others)

## [0.3.0] - 2019-12-03

### Added

- Support for Anytone BP-01 Bluetooth PTT Button

### Changed

- Partition layout (Please consider when upgrading from firmware <0.3.0)
- Fix answer to AT+IPHONEACCEV message
- Fix answer to AT+CSRSF message

## [0.2.1] - 2019-11-23

### Added

- Support for Fontastic NAT+ Bluetooth Headset
- Configuration Webinterface
  - ADC and DAC Gain
  - Bluetooth PIN Code
  - Reset Bluetooth Pairings
  - PTT Hang Time
- Display ESP32 ChipID during boot

### Changed

- Using AsyncWebServer library
- Firmware Update result is displayed properly
- Fixed Website layout for mobile

### Removed

## [0.2.0] - 2019-10-29

### Compatiblity

- dev-board v4.1

### Added

- dev-board v4.1 compatibility
- Showing hardware version during boot
- ESP32 compatibility
- WLAN Firmware Updater [#14]
- Add lint script (tools/code-style.sh)
- "Soft-PTT" - only working when PTT jumper is set to "uC"
  - Start a phone call when PTT is pressed [#10]
  - Keep PTT activated for some time after releasing the button [#11]
  - PTT LED shows state of PTT (on/off) [#9]
- Generate BT friendly name from BT address [#2]

### Changed

- Fixed memory allocation for serial input

### Removed

- Official Teensy3.2 compatibility

## [0.1.2] - 2019-08-11

### Added

- Build setup for ESP32
- Added CHANGELOG.md
- Add pin assignments for PTT pins

### Changed

- Phonebook works for VW now (using internal 'ME' phonebook instead of SIM 'SM')
- Asynchronuous detection of incoming BT connections (Bluetooth inquiry is not blocking anymore)

## [0.1.1] - 2019-07-28

### Added

- Travis and Appveyor CI Setup
- Added AT commands:
  - AT+GMM, AT+GMI
  - AT+APLSIRI
  - AT+CNMI=...
  - ATI
  - AT+CSRSF
- Indicating a signal level of 5 after HFP-AG connection

### Changed

- Using *BT PAIR 1 0* to be able to use authenticated pairing
- Handling *SSP CONFIRM* message during connectHFPAG()
- Do not automatically switch to data mode after new RFCOMM connection

### Removed

- none

## 0.1.0 - 2019-05-18

Initial public release

[1.1.0-rc2]: https://github.com/bt-trx/firmware/compare/1.1.0-rc1...1.1.0-rc2
[1.1.0-rc1]: https://github.com/bt-trx/firmware/compare/1.0.1...1.1.0-rc1
[1.0.1]: https://github.com/bt-trx/firmware/compare/1.0.0...1.0.1
[1.0.0]: https://github.com/bt-trx/firmware/compare/1.0.0-rc1...1.0.0
[1.0.0-rc1]: https://github.com/bt-trx/firmware/compare/0.3.1-rc1...1.0.0-rc1
[0.3.1-rc1]: https://github.com/bt-trx/firmware/compare/0.3.0...0.3.1-rc1
[0.3.0]: https://github.com/bt-trx/firmware/compare/0.2.1...0.3.0
[0.2.1]: https://github.com/bt-trx/firmware/compare/0.2.0...0.2.1
[0.2.0]: https://github.com/bt-trx/firmware/compare/0.1.2...0.2.0
[0.1.2]: https://github.com/bt-trx/firmware/compare/0.1.1...0.1.2
[0.1.1]: https://github.com/bt-trx/firmware/compare/0.1.0...0.1.1
