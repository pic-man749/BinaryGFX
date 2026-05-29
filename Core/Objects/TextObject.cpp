/*
 * TextObject.cpp
 *
 *      Author: picman
 */

#include "TextObject.hpp"
#include "../FrameBuffer.hpp"

namespace BinaryGFX {

  TextObject::TextObject(int16_t x, int16_t y, const char *text, const FontData *font, PixelState pixelState, int16_t z) :
      IGraphicsObject(z), m_x(x), m_y(y), m_text(text), m_font(font), m_pixelState(pixelState), m_charSpacing(1u), m_lineSpacing(1u), m_wordWrap(true) {
  }

  void TextObject::render(FrameBuffer &fb) const {
    if(m_text == nullptr || m_font == nullptr) {
      return;
    }

    int16_t curX = m_x;
    int16_t curY = m_y;
    const char *p = m_text;
    const int32_t fbWidth = static_cast<int32_t>(fb.getWidth());
    const uin8_t lastChar = m_font->firstChar + m_font->charCount;

    while(*p != '\0') {
      const uint8_t c = static_cast<uint8_t>(*p++);

      if(c == '\n') {
        curX = m_x;
        curY = static_cast<int16_t>(curY + m_font->glyphHeight + m_lineSpacing);
        continue;
      }

      // ワードラップ判定: 次の文字が画面幅を超える場合に折り返す
      if(m_wordWrap && (static_cast<int32_t>(curX) + m_font->glyphWidth > fbWidth)) {
        curX = m_x;
        curY = static_cast<int16_t>(curY + m_font->glyphHeight + m_lineSpacing);
      }

      // 対応文字コード範囲のみ描画
      if(m_font->firstChar <= c && c < lastChar) {
        drawGlyph(fb, curX, curY, c);
      }

      curX = static_cast<int16_t>(curX + m_font->glyphWidth + m_charSpacing);
    }
  }

  void TextObject::drawGlyph(FrameBuffer &fb, int16_t x, int16_t y, uint8_t c) const {
    const uint8_t glyphIndex = static_cast<uint8_t>(c - m_font->firstChar);
    const uint8_t pagesPerCol = static_cast<uint8_t>((m_font->glyphHeight + 7u) / 8u);
    const uint16_t bytesPerGlyph = static_cast<uint16_t>(m_font->glyphWidth) * pagesPerCol;
    const uint32_t glyphOffset = static_cast<uint32_t>(glyphIndex) * bytesPerGlyph;

    for(uint8_t col = 0u; col < m_font->glyphWidth; ++col) {
      for(uint8_t page = 0u; page < pagesPerCol; ++page) {
        const uint8_t byte = m_font->data[glyphOffset + col * pagesPerCol + page];
        for(uint8_t bit = 0u; bit < 8u; ++bit) {
          const uint8_t row = static_cast<uint8_t>(page * 8u + bit);
          if(row >= m_font->glyphHeight) {
            break;
          }
          if((byte >> bit) & 0x01u) {
            fb.setPixel(static_cast<int16_t>(x + col), static_cast<int16_t>(y + row), m_pixelState);
          }
        }
      }
    }
  }

  void TextObject::setPosition(int16_t x, int16_t y) {
    m_x = x;
    m_y = y;
  }

  void TextObject::setText(const char *text) {
    m_text = text;
  }

  void TextObject::setFont(const FontData *font) {
    m_font = font;
  }

  void TextObject::setPixelState(PixelState pixelState) {
    m_pixelState = pixelState;
  }

  void TextObject::setCharSpacing(uint8_t spacing) {
    m_charSpacing = spacing;
  }

  void TextObject::setLineSpacing(uint8_t spacing) {
    m_lineSpacing = spacing;
  }

  void TextObject::setWordWrap(bool enable) {
    m_wordWrap = enable;
  }

} // namespace BinaryGFX
