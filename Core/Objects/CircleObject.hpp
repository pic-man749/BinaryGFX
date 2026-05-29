/*
 * CircleObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_CIRCLEOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_CIRCLEOBJECT_HPP_

#include "IGraphicsObject.hpp"
#include "../../Common/PixelState.hpp"

namespace BinaryGFX {

  /**
   * @brief 円オブジェクト
   *
   * 中点円アルゴリズムを用いて円の枠線または塗りつぶし円を描画するグラフィックオブジェクト。
   */
  class CircleObject: public IGraphicsObject {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param cx     中心X座標
       * @param cy     中心Y座標
       * @param r      半径（ピクセル）
       * @param pixelState 点灯状態（デフォルト: 点灯）
       * @param filled 塗りつぶし有無（デフォルト: 枠線のみ）
       * @param z      Z値（デフォルト: 0）
       */
      CircleObject(int16_t cx, int16_t cy, int16_t r,
                   PixelState pixelState = PixelState::On, bool filled = false, int16_t z = 0);

      /**
       * @brief フレームバッファへ自身を描画する
       *
       * @param fb 描画先フレームバッファ
       */
      void render(FrameBuffer &fb) const override;

      /**
       * @brief 中心座標を設定する
       *
       * @param cx 中心X座標
       * @param cy 中心Y座標
       */
      void setCenter(int16_t cx, int16_t cy);

      /**
       * @brief 半径を設定する
       *
       * @param r 半径（ピクセル）
       */
      void setRadius(int16_t r);

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

      int16_t m_cx;             /**< 中心X座標 */
      int16_t m_cy;             /**< 中心Y座標 */
      int16_t m_r;              /**< 半径 */
      PixelState m_pixelState;  /**< 点灯状態 */
      bool m_filled;            /**< 塗りつぶし有無 */

      /**
       * @brief 水平ラインを描画するヘルパー
       *
       * @param fb          描画先フレームバッファ
       * @param x0          始点X座標
       * @param x1          終点X座標
       * @param y           Y座標
       * @param pixelState  点灯状態
       */
      static void drawHLine(FrameBuffer &fb, int16_t x0, int16_t x1, int16_t y, PixelState pixelState);
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_CIRCLEOBJECT_HPP_ */
