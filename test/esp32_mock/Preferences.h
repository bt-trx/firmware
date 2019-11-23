#pragma once

#include <gmock/gmock.h>

class Preferences {
  public:
  MOCK_METHOD2(getUShort, uint16_t(const char*, uint16_t));
  MOCK_METHOD2(putUShort, size_t(const char*, uint16_t));
};