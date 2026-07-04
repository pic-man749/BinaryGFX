/*
 * TextObject.cpp
 *
 *      Author: picman
 */

#include "TextObject.hpp"

namespace BinaryGFX {

  TextObject::TextObject(int16_t x, int16_t y, const char *text, const FontData *font,
      PixelState pixelState, int16_t z) :
      TextObjectBase(x, y, font, pixelState, z), m_text(text) {
  }

  void TextObject::setText(const char *text) {
    m_text = text;
  }

  const char* TextObject::textPtr() const {
    return m_text;
  }

} // namespace BinaryGFX
