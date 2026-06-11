/*
 * BinaryObject.cpp
 *
 *      Author: picman
 */

#include "BinaryObject.hpp"
#include "../FrameBuffer.hpp"

namespace BinaryGFX {

  BinaryObject::BinaryObject(int16_t x, int16_t y, const BinaryData *data,
      PixelState pixelState, int16_t z) :
      IGraphicsObject(z), m_x(x), m_y(y), m_data(data), m_pixelState(pixelState) {
  }

  void BinaryObject::render(FrameBuffer &fb) const {
    if(m_data == nullptr || m_data->data == nullptr) {
      return;
    }

    const uint8_t pagesPerCol = static_cast<uint8_t>((m_data->height + 7u) / PIXELS_PER_BYTE);

    for(uint16_t col = 0u; col < m_data->width; ++col) {
      for(uint8_t page = 0u; page < pagesPerCol; ++page) {
        const uint8_t byte = m_data->data[col * pagesPerCol + page];
        if(byte == 0x00u) continue;
        fb.setPage(
            static_cast<int16_t>(m_x + static_cast<int16_t>(col)),
            static_cast<int16_t>(m_y + static_cast<int16_t>(page) * static_cast<int16_t>(PIXELS_PER_BYTE)),
            byte, m_pixelState);
      }
    }
  }

  void BinaryObject::setPosition(int16_t x, int16_t y) {
    m_x = x;
    m_y = y;
  }

  void BinaryObject::setBinaryData(const BinaryData *data) {
    m_data = data;
  }

  void BinaryObject::setPixelState(PixelState pixelState) {
    m_pixelState = pixelState;
  }

} // namespace BinaryGFX
