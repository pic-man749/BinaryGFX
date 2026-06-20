/*
 * RectangleObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_RECTANGLEOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_RECTANGLEOBJECT_HPP_

#include "../IGraphicsObject.hpp"
#include "../../Common/PixelState.hpp"

namespace BinaryGFX {

  /**
   * @brief 矩形オブジェクト
   *
   * 矩形の枠線または塗りつぶし矩形を描画するグラフィックオブジェクト。
   */
  class RectangleObject: public IGraphicsObject {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param x      左上X座標
       * @param y      左上Y座標
       * @param w      幅（ピクセル）
       * @param h      高さ（ピクセル）
       * @param pixelState 点灯状態（デフォルト: 点灯）
       * @param filled 塗りつぶし有無（デフォルト: 枠線のみ）
       * @param z      Z値（デフォルト: 0）
       */
      RectangleObject(int16_t x, int16_t y, int16_t w, int16_t h, PixelState pixelState = PixelState::On, bool filled = false, int16_t z = 0);

      /**
       * @brief フレームバッファへ自身を描画する
       *
       * @param fb 描画先フレームバッファ
       */
      void render(FrameBuffer &fb) const override;

      /**
       * @brief 左上座標を設定する
       *
       * @param x 左上X座標
       * @param y 左上Y座標
       */
      void setPosition(int16_t x, int16_t y);

      /**
       * @brief サイズを設定する
       *
       * @param w 幅（ピクセル）
       * @param h 高さ（ピクセル）
       */
      void setSize(int16_t w, int16_t h);

      /**
       * @brief 点灯状態を設定する
       *
       * @param pixelState 点灯状態
       */
      void setPixelState(PixelState pixelState);

      /**
       * @brief 塗りつぶし有無を設定する
       *
       * @param filled true:塗りつぶし、false:枠線のみ
       */
      void setFilled(bool filled);

    private:

      int16_t m_x;             /**< 左上X座標 */
      int16_t m_y;             /**< 左上Y座標 */
      int16_t m_w;             /**< 幅 */
      int16_t m_h;             /**< 高さ */
      PixelState m_pixelState; /**< 点灯状態 */
      bool m_filled;           /**< 塗りつぶし有無 */
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_RECTANGLEOBJECT_HPP_ */
