/*
 * TriangleObject.cpp
 *
 *      Author: picman
 */

#include <cstdlib>
#include <cstdint>
#include "TriangleObject.hpp"
#include "../FrameBuffer.hpp"

namespace BinaryGFX {

  TriangleObject::TriangleObject(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, PixelState pixelState, bool filled, int16_t z) :
      IGraphicsObject(z), m_x0(x0), m_y0(y0), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_pixelState(pixelState), m_filled(filled) {
  }

  void TriangleObject::render(FrameBuffer &fb) const {
    if(!m_filled) {
      drawLine(fb, m_x0, m_y0, m_x1, m_y1, m_pixelState);
      drawLine(fb, m_x1, m_y1, m_x2, m_y2, m_pixelState);
      drawLine(fb, m_x2, m_y2, m_x0, m_y0, m_pixelState);
      return;
    }

    // 頂点をY値でソート（バブルソート: ay <= by <= cy）
    int16_t ax = m_x0, ay = m_y0;
    int16_t bx = m_x1, by = m_y1;
    int16_t cx = m_x2, cy = m_y2;
    int16_t tmp;

    if(ay > by) {
      tmp = ax;
      ax = bx;
      bx = tmp;
      tmp = ay;
      ay = by;
      by = tmp;
    }
    if(ay > cy) {
      tmp = ax;
      ax = cx;
      cx = tmp;
      tmp = ay;
      ay = cy;
      cy = tmp;
    }
    if(by > cy) {
      tmp = bx;
      bx = cx;
      cx = tmp;
      tmp = by;
      by = cy;
      cy = tmp;
    }

    // スキャンラインで塗りつぶし
    for(int16_t y = ay; y <= cy; y++) {
      // 長辺（頂点a-c）上のX座標
      const int16_t xAC = lerpX(ax, ay, cx, cy, y);
      // 短辺のX座標: ay〜by区間はa-b辺、by〜cy区間はb-c辺
      const int16_t xOther = (y <= by) ? lerpX(ax, ay, bx, by, y) : lerpX(bx, by, cx, cy, y);

      const int16_t xLeft = (xAC < xOther) ? xAC : xOther;
      const int16_t xRight = (xAC > xOther) ? xAC : xOther;
      for(int16_t x = xLeft; x <= xRight; x++) {
        fb.setPixel(x, y, m_pixelState);
      }
    }
  }

  void TriangleObject::setVertices(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    m_x0 = x0;
    m_y0 = y0;
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
  }

  void TriangleObject::setPixelState(PixelState pixelState) {
    m_pixelState = pixelState;
  }

  void TriangleObject::setFilled(bool filled) {
    m_filled = filled;
  }

  void TriangleObject::drawLine(FrameBuffer &fb, int16_t x0, int16_t y0, int16_t x1, int16_t y1, PixelState pixelState) {
    // Bresenhamの直線描画アルゴリズム
    const int16_t dx = static_cast<int16_t>(std::abs(x1 - x0));
    const int16_t dy = static_cast<int16_t>(-std::abs(y1 - y0));
    const int16_t sx = (x0 < x1) ? static_cast<int16_t>(1) : static_cast<int16_t>(-1);
    const int16_t sy = (y0 < y1) ? static_cast<int16_t>(1) : static_cast<int16_t>(-1);
    int16_t err = static_cast<int16_t>(dx + dy);
    int16_t x = x0;
    int16_t y = y0;

    while(true) {
      fb.setPixel(x, y, pixelState);
      if(x == x1 && y == y1) {
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

  int16_t TriangleObject::lerpX(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t y) {
    // 水平辺の場合は終点X座標を返す（平頭・平底三角形への対応）
    if(y1 == y0) {
      return x1;
    }
    return static_cast<int16_t>(x0 + (static_cast<int32_t>(y - y0) * (x1 - x0)) / (y1 - y0));
  }

} // namespace BinaryGFX
