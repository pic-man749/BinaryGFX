/*
 * TextObjectBase.cpp
 *
 *      Author: picman
 */

#include "TextObjectBase.hpp"
#include "../FrameBuffer.hpp"

namespace BinaryGFX {

  TextObjectBase::TextObjectBase(int16_t x, int16_t y, const FontData *font,
                                 PixelState pixelState,
                                 int16_t z) :
      IGraphicsObject(z), m_x(x), m_y(y), m_font(font), m_pixelState(pixelState),
      m_charSpacing(1u),
      m_lineSpacing(0u), m_wordWrap(true), m_width(0u), m_align(TextAlign::Left) {
    updateFontMetrics();
  }

  void TextObjectBase::render(FrameBuffer &fb) const {
    const char *text = textPtr();
    if(text == nullptr || m_font == nullptr) {
      return;
    }

    const int32_t fbWidth = static_cast<int32_t>(fb.getWidth());
    const int32_t boxLeft = m_x;
    const int32_t boxWidth = (m_width == 0u) ? (fbWidth - boxLeft) : static_cast<int32_t>(m_width);
    const int32_t boxRight = boxLeft + boxWidth;

    int16_t curY = m_y;
    const char *lineStart = text;

    while(*lineStart != '\0') {
      const char *lineEnd = nullptr;
      const char *nextStart = nullptr;
      const int32_t lineWidth = findLineEnd(lineStart, boxLeft, boxRight, &lineEnd, &nextStart);

      const int16_t startX = computeAlignedX(boxLeft, boxWidth, lineWidth);
      drawLineChars(fb, lineStart, lineEnd, startX, curY);

      curY = static_cast<int16_t>(curY + m_font->glyphHeight + m_lineSpacing);
      lineStart = nextStart;
    }
  }

  int32_t TextObjectBase::findLineEnd(const char *lineStart, int32_t boxLeft, int32_t boxRight,
                                      const char **lineEnd,
                                      const char **nextStart) const {
    int32_t rawWidth = 0;
    bool hasVisibleChar = false;
    const uint8_t lastChar = static_cast<uint8_t>(m_font->firstChar + m_font->charCount);
    const char *p = lineStart;

    while(*p != '\0') {
      const char *charPos = p;
      const uint8_t c = static_cast<uint8_t>(*p);

      if(c == '\n') {
        *lineEnd = charPos;
        *nextStart = charPos + 1;
        return hasVisibleChar ? (rawWidth - m_charSpacing) : 0;
      }

      // \r\n 改行コードに対応するため \r をスキップ（カーソルは進めない）
      if(c == '\r') {
        ++p;
        continue;
      }

      // 対応文字コード範囲外の文字はカーソルを進めずスキップ
      if(c < m_font->firstChar || c >= lastChar) {
        ++p;
        continue;
      }

      // ワードラップ判定: 行に既に1文字以上配置済みで、次の文字がボックス幅を超える場合に折り返す
      // （行の先頭文字は、ボックスより広い場合でも必ず1文字は配置し前進させる）
      if(m_wordWrap && hasVisibleChar && (boxLeft + rawWidth + m_font->glyphWidth > boxRight)) {
        *lineEnd = charPos;
        *nextStart = charPos;
        return rawWidth - m_charSpacing;
      }

      rawWidth += static_cast<int32_t>(m_font->glyphWidth) + m_charSpacing;
      hasVisibleChar = true;
      ++p;
    }

    *lineEnd = p;
    *nextStart = p;
    return hasVisibleChar ? (rawWidth - m_charSpacing) : 0;
  }

  void TextObjectBase::drawLineChars(FrameBuffer &fb, const char *start, const char *end, int16_t x, int16_t y) const {
    int16_t curX = x;
    const uint8_t lastChar = static_cast<uint8_t>(m_font->firstChar + m_font->charCount);

    for(const char *p = start; p != end; ++p) {
      const uint8_t c = static_cast<uint8_t>(*p);

      // \r\n 改行コードに対応するため \r をスキップ
      if(c == '\r') {
        continue;
      }

      // 対応文字コード範囲外の文字はカーソルを進めずスキップ
      if(c < m_font->firstChar || c >= lastChar) {
        continue;
      }

      drawGlyph(fb, curX, y, c);
      curX = static_cast<int16_t>(curX + m_font->glyphWidth + m_charSpacing);
    }
  }

  int16_t TextObjectBase::computeAlignedX(int32_t boxLeft, int32_t boxWidth, int32_t lineWidth) const {
    int32_t offset = 0;
    switch(m_align) {
      case TextAlign::Center:
        offset = (boxWidth - lineWidth) / 2;
        break;
      case TextAlign::Right:
        offset = boxWidth - lineWidth;
        break;
      case TextAlign::Left:
        default:
        offset = 0;
        break;
    }
    return static_cast<int16_t>(boxLeft + offset);
  }

  void TextObjectBase::drawGlyph(FrameBuffer &fb, int16_t x, int16_t y, uint8_t c) const {
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

  void TextObjectBase::setPosition(int16_t x, int16_t y) {
    m_x = x;
    m_y = y;
  }

  void TextObjectBase::setFont(const FontData *font) {
    m_font = font;
    updateFontMetrics();
  }

  void TextObjectBase::updateFontMetrics() {
    if(m_font == nullptr) {
      m_pagesPerCol = 0u;
      m_bytesPerGlyph = 0u;
      return;
    }
    // +7 は切り上げ除算（ceil(h/8)）のオフセット
    m_pagesPerCol = static_cast<uint8_t>((m_font->glyphHeight + 7u) / PIXELS_PER_BYTE);
    m_bytesPerGlyph = static_cast<uint16_t>(m_font->glyphWidth) * m_pagesPerCol;
  }

  void TextObjectBase::setPixelState(PixelState pixelState) {
    m_pixelState = pixelState;
  }

  void TextObjectBase::setCharSpacing(uint8_t spacing) {
    m_charSpacing = spacing;
  }

  void TextObjectBase::setLineSpacing(uint8_t spacing) {
    m_lineSpacing = spacing;
  }

  void TextObjectBase::setWordWrap(bool enable) {
    m_wordWrap = enable;
  }

  void TextObjectBase::setWidth(uint16_t width) {
    m_width = width;
  }

  void TextObjectBase::setAlign(TextAlign align) {
    m_align = align;
  }

} // namespace BinaryGFX
