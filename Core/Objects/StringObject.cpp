/*
 * StringObject.cpp
 *
 *      Author: picman
 */

#include "StringObject.hpp"

namespace BinaryGFX {

  StringObject::StringObject(int16_t x, int16_t y, const std::string string, const FontData *font,
      PixelState pixelState, int16_t z) :
      TextObjectBase(x, y, font, pixelState, z), m_string(string) {
  }

  void StringObject::setText(const std::string string) {
    m_string = string;
  }

  std::string& StringObject::getText() {
    return m_string;
  }

  const char* StringObject::textPtr() const {
    return m_string.c_str();
  }

} // namespace BinaryGFX
