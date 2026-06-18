/*
 * StringObject.cpp
 *
 *      Author: picman
 */

#include "StringObject.hpp"
#include "../FrameBuffer.hpp"

namespace BinaryGFX {

  StringObject::StringObject(int16_t x, int16_t y, const std::string string, const FontData *font,
      PixelState pixelState, int16_t z) :
      IGraphicsObject(z), m_x(x), m_y(y), m_string(string), m_font(font), m_pixelState(pixelState),
      m_charSpacing(1u), m_lineSpacing(0u), m_wordWrap(true) {
    updateFontMetrics();
  }

  void StringObject::render(FrameBuffer &fb) const {
    if(m_string.empty() || m_font == nullptr) {
      return;
    }

    int16_t curX = m_x;
    int16_t curY = m_y;
    const char *p = m_string.c_str();
    const int32_t fbWidth = static_cast<int32_t>(fb.getWidth());
    const uint8_t lastChar = m_font->firstChar + m_font->charCount;

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

  void StringObject::drawGlyph(FrameBuffer &fb, int16_t x, int16_t y, uint8_t c) const {
    const uint8_t glyphIndex = static_cast<uint8_t>(c - m_font->firstChar);
    const uint32_t glyphOffset = static_cast<uint32_t>(glyphIndex) * m_bytesPerGlyph;

    for(uint8_t col = 0u; col < m_font->glyphWidth; ++col) {
      for(uint8_t page = 0u; page < m_pagesPerCol; ++page) {
        const uint8_t byte = m_font->data[glyphOffset + col * m_pagesPerCol + page];
        if(byte == 0x00u) continue; // 空白列はスキップ
        fb.setPage(static_cast<int16_t>(x + col), static_cast<int16_t>(y + page * PIXELS_PER_BYTE),
            byte, m_pixelState);
      }
    }
  }

  void StringObject::setPosition(int16_t x, int16_t y) {
    m_x = x;
    m_y = y;
  }

  void StringObject::setText(const std::string string) {
    m_string = string;
  }

  void StringObject::setFont(const FontData *font) {
    m_font = font;
    updateFontMetrics();
  }

  void StringObject::updateFontMetrics() {
    if(m_font == nullptr) {
      m_pagesPerCol = 0u;
      m_bytesPerGlyph = 0u;
      return;
    }
    // +7 は切り上げ除算（ceil(h/8)）のオフセット
    m_pagesPerCol = static_cast<uint8_t>((m_font->glyphHeight + 7u) / PIXELS_PER_BYTE);
    m_bytesPerGlyph = static_cast<uint16_t>(m_font->glyphWidth) * m_pagesPerCol;
  }

  void StringObject::setPixelState(PixelState pixelState) {
    m_pixelState = pixelState;
  }

  void StringObject::setCharSpacing(uint8_t spacing) {
    m_charSpacing = spacing;
  }

  void StringObject::setLineSpacing(uint8_t spacing) {
    m_lineSpacing = spacing;
  }

  void StringObject::setWordWrap(bool enable) {
    m_wordWrap = enable;
  }

} // namespace BinaryGFX
