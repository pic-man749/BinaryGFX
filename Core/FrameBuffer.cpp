/*
 * FrameBuffer.cpp
 *
 *      Author: picman
 */

#include "../Inc/FrameBuffer.hpp"

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
