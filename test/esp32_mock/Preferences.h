#pragma once

#include <gmock/gmock.h>

class Preferences {
  public:
  MOCK_METHOD2(getUShort, uint16_t(const char*, uint16_t));
  MOCK_METHOD2(putUShort, size_t(const char*, uint16_t));
  MOCK_METHOD2(getBool, size_t(const char*, bool));
  MOCK_METHOD2(putBool, size_t(const char*, bool));
  MOCK_METHOD2(getString, string(const char*, const char*));
  MOCK_METHOD2(putString, size_t(const char*, const char*));
};
