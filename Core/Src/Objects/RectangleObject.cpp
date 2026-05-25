/*
 * RectangleObject.cpp
 *
 *      Author: picman
 */

#include "../../Inc/Objects/RectangleObject.hpp"
#include "../../Inc/FrameBuffer.hpp"

namespace BinaryGFX {

  RectangleObject::RectangleObject(int16_t x, int16_t y, int16_t w, int16_t h, PixelState pixelState, bool filled, int16_t z) :
      IGraphicsObject(z), m_x(x), m_y(y), m_w(w), m_h(h), m_pixelState(pixelState), m_filled(filled) {
  }

  void RectangleObject::render(FrameBuffer &fb) const {
    if(m_w <= 0 || m_h <= 0) {
      return;
    }
    const int16_t x1 = static_cast<int16_t>(m_x + m_w - 1);
    const int16_t y1 = static_cast<int16_t>(m_y + m_h - 1);

    if(m_filled) {
      for(int16_t y = m_y; y <= y1; y++) {
        for(int16_t x = m_x; x <= x1; x++) {
          fb.setPixel(x, y, m_pixelState);
        }
      }
    } else {
      // 上辺・下辺
      for(int16_t x = m_x; x <= x1; x++) {
        fb.setPixel(x, m_y, m_pixelState);
        fb.setPixel(x, y1, m_pixelState);
      }
      // 左辺・右辺（角は上下辺で描画済み）
      for(int16_t y = static_cast<int16_t>(m_y + 1); y < y1; y++) {
        fb.setPixel(m_x, y, m_pixelState);
        fb.setPixel(x1, y, m_pixelState);
      }
    }
  }

  void RectangleObject::setPosition(int16_t x, int16_t y) {
    m_x = x;
    m_y = y;
  }

  void RectangleObject::setSize(int16_t w, int16_t h) {
    m_w = w;
    m_h = h;
  }

  void RectangleObject::setPixelState(PixelState pixelState) {
    m_pixelState = pixelState;
  }

  void RectangleObject::setFilled(bool filled) {
    m_filled = filled;
  }

} // namespace BinaryGFX
