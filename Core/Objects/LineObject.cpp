/*
 * LineObject.cpp
 *
 *      Author: picman
 */

#include <cstdlib>
#include "LineObject.hpp"
#include "../FrameBuffer.hpp"

namespace BinaryGFX {

  LineObject::LineObject(int16_t x0, int16_t y0, int16_t x1, int16_t y1, PixelState pixelState, int16_t z) :
      IGraphicsObject(z), m_x0(x0), m_y0(y0), m_x1(x1), m_y1(y1), m_pixelState(pixelState) {
  }

  void LineObject::render(FrameBuffer &fb) const {
    // Bresenhamの直線描画アルゴリズム
    const int16_t dx = static_cast<int16_t>(std::abs(m_x1 - m_x0));
    const int16_t dy = static_cast<int16_t>(-std::abs(m_y1 - m_y0));
    const int16_t sx = (m_x0 < m_x1) ? static_cast<int16_t>(1) : static_cast<int16_t>(-1);
    const int16_t sy = (m_y0 < m_y1) ? static_cast<int16_t>(1) : static_cast<int16_t>(-1);
    int16_t err = static_cast<int16_t>(dx + dy);
    int16_t x = m_x0;
    int16_t y = m_y0;

    while(true) {
      fb.setPixel(x, y, m_pixelState);
      if(x == m_x1 && y == m_y1) {
        break;
      }
      const int16_t e2 = static_cast<int16_t>(2 * err);
      if(e2 >= dy) {
        err = static_cast<int16_t>(err + dy);
        x = static_cast<int16_t>(x + sx);
      }
      if(e2 <= dx) {
        err = static_cast<int16_t>(err + dx);
        y = static_cast<int16_t>(y + sy);
      }
    }
  }

  void LineObject::setStart(int16_t x0, int16_t y0) {
    m_x0 = x0;
    m_y0 = y0;
  }

  void LineObject::setEnd(int16_t x1, int16_t y1) {
    m_x1 = x1;
    m_y1 = y1;
  }

  void LineObject::setPixelState(PixelState pixelState) {
    m_pixelState = pixelState;
  }

} // namespace BinaryGFX
