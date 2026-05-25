/*
 * CircleObject.cpp
 *
 *      Author: picman
 */

#include "../../Inc/Objects/CircleObject.hpp"
#include "../../Inc/FrameBuffer.hpp"

namespace BinaryGFX {

  CircleObject::CircleObject(int16_t cx, int16_t cy, int16_t r, PixelState pixelState, bool filled, int16_t z) :
      IGraphicsObject(z), m_cx(cx), m_cy(cy), m_r(r), m_pixelState(pixelState), m_filled(filled) {
  }

  void CircleObject::render(FrameBuffer &fb) const {
    if(m_r < 0) {
      return;
    }

    // 中点円アルゴリズム
    int16_t x = 0;
    int16_t y = m_r;
    int16_t d = static_cast<int16_t>(1 - m_r);

    while(x <= y) {
      if(m_filled) {
        // スキャンライン水平線で8方向対称に塗りつぶす
        drawHLine(fb, static_cast<int16_t>(m_cx - x), static_cast<int16_t>(m_cx + x), static_cast<int16_t>(m_cy + y), m_pixelState);
        drawHLine(fb, static_cast<int16_t>(m_cx - x), static_cast<int16_t>(m_cx + x), static_cast<int16_t>(m_cy - y), m_pixelState);
        drawHLine(fb, static_cast<int16_t>(m_cx - y), static_cast<int16_t>(m_cx + y), static_cast<int16_t>(m_cy + x), m_pixelState);
        drawHLine(fb, static_cast<int16_t>(m_cx - y), static_cast<int16_t>(m_cx + y), static_cast<int16_t>(m_cy - x), m_pixelState);
      } else {
        fb.setPixel(static_cast<int16_t>(m_cx + x), static_cast<int16_t>(m_cy + y), m_pixelState);
        fb.setPixel(static_cast<int16_t>(m_cx - x), static_cast<int16_t>(m_cy + y), m_pixelState);
        fb.setPixel(static_cast<int16_t>(m_cx + x), static_cast<int16_t>(m_cy - y), m_pixelState);
        fb.setPixel(static_cast<int16_t>(m_cx - x), static_cast<int16_t>(m_cy - y), m_pixelState);
        fb.setPixel(static_cast<int16_t>(m_cx + y), static_cast<int16_t>(m_cy + x), m_pixelState);
        fb.setPixel(static_cast<int16_t>(m_cx - y), static_cast<int16_t>(m_cy + x), m_pixelState);
        fb.setPixel(static_cast<int16_t>(m_cx + y), static_cast<int16_t>(m_cy - x), m_pixelState);
        fb.setPixel(static_cast<int16_t>(m_cx - y), static_cast<int16_t>(m_cy - x), m_pixelState);
      }

      if(d < 0) {
        d = static_cast<int16_t>(d + 2 * x + 3);
      } else {
        d = static_cast<int16_t>(d + 2 * (x - y) + 5);
        y = static_cast<int16_t>(y - 1);
      }
      x = static_cast<int16_t>(x + 1);
    }
  }

  void CircleObject::setCenter(int16_t cx, int16_t cy) {
    m_cx = cx;
    m_cy = cy;
  }

  void CircleObject::setRadius(int16_t r) {
    m_r = r;
  }

  void CircleObject::setPixelState(PixelState pixelState) {
    m_pixelState = pixelState;
  }

  void CircleObject::setFilled(bool filled) {
    m_filled = filled;
  }

  void CircleObject::drawHLine(FrameBuffer &fb, int16_t x0, int16_t x1, int16_t y, PixelState pixelState) {
    for(int16_t x = x0; x <= x1; x++) {
      fb.setPixel(x, y, pixelState);
    }
  }

} // namespace BinaryGFX
