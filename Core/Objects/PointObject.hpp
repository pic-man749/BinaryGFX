/*
 * PointObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_POINTOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_POINTOBJECT_HPP_

#include "../IGraphicsObject.hpp"
#include "../../Common/PixelState.hpp"

namespace BinaryGFX {

  /**
   * @brief 点オブジェクト
   *
   * 単一ピクセルを描画するグラフィックオブジェクト。
   */
  class PointObject: public IGraphicsObject {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param x    X座標
       * @param y    Y座標
       * @param pixelState 点灯状態（デフォルト: 点灯）
       * @param z    Z値（デフォルト: 0）
       */
      PointObject(int16_t x, int16_t y, PixelState pixelState = PixelState::On, int16_t z = 0);

      /**
       * @brief フレームバッファへ自身を描画する
       *
       * @param fb 描画先フレームバッファ
       */
      void render(FrameBuffer &fb) const override;

      /**
       * @brief 座標を設定する
       *
       * @param x X座標
       * @param y Y座標
       */
      void setPosition(int16_t x, int16_t y);

      /**
       * @brief 点灯状態を設定する
       *
       * @param pixelState 点灯状態
       */
      void setPixelState(PixelState pixelState);

    private:

      int16_t m_x;             /**< X座標 */
      int16_t m_y;             /**< Y座標 */
      PixelState m_pixelState; /**< 点灯状態 */
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_POINTOBJECT_HPP_ */
