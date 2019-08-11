# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
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

[Unreleased]: https://github.com/bt-trx/firmware/compare/0.1.1...HEAD
[0.1.1]: https://github.com/bt-trx/firmware/compare/0.1.0...0.1.1