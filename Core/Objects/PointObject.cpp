/*
 * PointObject.cpp
 *
 *      Author: picman
 */

#include "PointObject.hpp"
#include "../FrameBuffer.hpp"

namespace BinaryGFX {

  PointObject::PointObject(int16_t x, int16_t y, PixelState pixelState, int16_t z) :
      IGraphicsObject(z), m_x(x), m_y(y), m_pixelState(pixelState) {
  }

  void PointObject::render(FrameBuffer &fb) const {
    fb.setPixel(m_x, m_y, m_pixelState);
  }

  void PointObject::setPosition(int16_t x, int16_t y) {
    m_x = x;
    m_y = y;
  }

  void PointObject::setPixelState(PixelState pixelState) {
    m_pixelState = pixelState;
  }

} // namespace BinaryGFX
