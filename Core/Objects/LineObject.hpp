/*
 * LineObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_LINEOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_LINEOBJECT_HPP_

#include "IGraphicsObject.hpp"
#include "../../Common/PixelState.hpp"

namespace BinaryGFX {

  /**
   * @brief 線オブジェクト
   *
   * Bresenhamの直線描画アルゴリズムを用いて線分を描画するグラフィックオブジェクト。
   */
  class LineObject: public IGraphicsObject {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param x0   始点X座標
       * @param y0   始点Y座標
       * @param x1   終点X座標
       * @param y1   終点Y座標
       * @param pixelState 点灯状態（デフォルト: 点灯）
       * @param z    Z値（デフォルト: 0）
       */
      LineObject(int16_t x0, int16_t y0, int16_t x1, int16_t y1, PixelState pixelState = PixelState::On, int16_t z = 0);

      /**
       * @brief フレームバッファへ自身を描画する
       *
       * @param fb 描画先フレームバッファ
       */
      void render(FrameBuffer &fb) const override;

      /**
       * @brief 始点を設定する
       *
       * @param x0 始点X座標
       * @param y0 始点Y座標
       */
      void setStart(int16_t x0, int16_t y0);

      /**
       * @brief 終点を設定する
       *
       * @param x1 終点X座標
       * @param y1 終点Y座標
       */
      void setEnd(int16_t x1, int16_t y1);

      /**
       * @brief 点灯状態を設定する
       *
       * @param pixelState 点灯状態
       */
      void setPixelState(PixelState pixelState);

    private:

      int16_t m_x0;             /**< 始点X座標 */
      int16_t m_y0;             /**< 始点Y座標 */
      int16_t m_x1;             /**< 終点X座標 */
      int16_t m_y1;             /**< 終点Y座標 */
      PixelState m_pixelState;  /**< 点灯状態 */
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_LINEOBJECT_HPP_ */
