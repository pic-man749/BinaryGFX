/*
 * FrameBuffer.cpp
 *
 *      Author: picman
 */

#include "FrameBuffer.hpp"

namespace BinaryGFX {

  FrameBuffer::FrameBuffer(uint16_t width, uint16_t height) :
      m_width(width), m_height(height), m_buffer(static_cast<size_t>(width) * (static_cast<size_t>(height) / 8u), 0x00u) {
  }

  void FrameBuffer::setPixel(int16_t x, int16_t y, PixelState ps) {
    if(x < 0 || x >= static_cast<int16_t>(m_width) || y < 0 || y >= static_cast<int16_t>(m_height)) {
      return;
    }
    const size_t index = (static_cast<size_t>(y) / 8u) * m_width + static_cast<size_t>(x);
    const uint8_t bit = static_cast<uint8_t>(1u << (static_cast<uint8_t>(y) % 8u));
    if(ps == PixelState::On) {
      m_buffer[index] |= bit;
    } else {
      m_buffer[index] &= static_cast<uint8_t>(~bit);
    }
  }

  void FrameBuffer::setPage(int16_t x, int16_t y, uint8_t byte, PixelState pixelState) {
    if(x < 0 || x >= static_cast<int16_t>(m_width) || y < 0) {
      return;
    }

    const size_t page0    = static_cast<size_t>(y) / PIXELS_PER_BYTE;
    const size_t bufPages = static_cast<size_t>(m_height) / PIXELS_PER_BYTE;
    if(page0 >= bufPages) {
      return;
    }

    const uint8_t offset = static_cast<uint8_t>(static_cast<uint8_t>(y) % PIXELS_PER_BYTE);
    const size_t  xIdx   = static_cast<size_t>(x);

    // 現在のページへの書き込み（byte を offset ビット上方シフト）
    const uint8_t loByte = static_cast<uint8_t>(byte << offset);
    if(pixelState == PixelState::On) {
      m_buffer[page0 * m_width + xIdx] |= loByte;
    } else {
      m_buffer[page0 * m_width + xIdx] &= static_cast<uint8_t>(~loByte);
    }

    // 非アライメント時は次のページにはみ出したビットを書き込む
    if(offset > 0u) {
      const size_t  page1  = page0 + 1u;
      const uint8_t hiByte = static_cast<uint8_t>(byte >> (PIXELS_PER_BYTE - offset));
      if(hiByte != 0x00u && page1 < bufPages) {
        if(pixelState == PixelState::On) {
          m_buffer[page1 * m_width + xIdx] |= hiByte;
        } else {
          m_buffer[page1 * m_width + xIdx] &= static_cast<uint8_t>(~hiByte);
        }
      }
    }
  }

  bool FrameBuffer::getPixel(int16_t x, int16_t y) const {
    if(x < 0 || x >= static_cast<int16_t>(m_width) || y < 0 || y >= static_cast<int16_t>(m_height)) {
      return false;
    }
    const size_t index = (static_cast<size_t>(y) / 8u) * m_width + static_cast<size_t>(x);
    const uint8_t bit = static_cast<uint8_t>(1u << (static_cast<uint8_t>(y) % 8u));
    return (m_buffer[index] & bit) != 0u;
  }

  void FrameBuffer::clear(PixelState pixelState) {
    const uint8_t value = (pixelState == PixelState::On) ? 0xFFu : 0x00u;
    m_buffer.assign(m_buffer.size(), value);
  }

  const uint8_t* FrameBuffer::getData() const {
    return m_buffer.data();
  }

  size_t FrameBuffer::getDataSize() const {
    return m_buffer.size();
  }

  uint16_t FrameBuffer::getWidth() const {
    return m_width;
  }

  uint16_t FrameBuffer::getHeight() const {
    return m_height;
  }

} // namespace BinaryGFX
