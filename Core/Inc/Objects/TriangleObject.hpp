/*
 * TriangleObject.hpp
 *
 *      Author: picman
 */

#ifndef USERLIB_BINARYGFX_CORE_INC_OBJECTS_TRIANGLEOBJECT_HPP_
#define USERLIB_BINARYGFX_CORE_INC_OBJECTS_TRIANGLEOBJECT_HPP_

#include "IGraphicsObject.hpp"
#include "../../../Common/Inc/PixelState.hpp"

namespace BinaryGFX {

  /**
   * @brief 三角形オブジェクト
   *
   * 三角形の枠線または塗りつぶし三角形を描画するグラフィックオブジェクト。
   * 枠線はBresenhamアルゴリズム、塗りつぶしはスキャンライン法で実装する。
   */
  class TriangleObject: public IGraphicsObject {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param x0     頂点0 X座標
       * @param y0     頂点0 Y座標
       * @param x1     頂点1 X座標
       * @param y1     頂点1 Y座標
       * @param x2     頂点2 X座標
       * @param y2     頂点2 Y座標
       * @param ps     点灯状態（デフォルト: 点灯）
       * @param filled 塗りつぶし有無（デフォルト: 枠線のみ）
       * @param z      Z値（デフォルト: 0）
       */
      TriangleObject(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, PixelState ps = PixelState::On, bool filled = false, int16_t z = 0);

      /**
       * @brief フレームバッファへ自身を描画する
       *
       * @param fb 描画先フレームバッファ
       */
      void render(FrameBuffer &fb) const override;

      /**
       * @brief 頂点座標を設定する
       *
       * @param x0 頂点0 X座標
       * @param y0 頂点0 Y座標
       * @param x1 頂点1 X座標
       * @param y1 頂点1 Y座標
       * @param x2 頂点2 X座標
       * @param y2 頂点2 Y座標
       */
      void setVertices(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

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

      int16_t m_x0;             /**< 頂点0 X座標 */
      int16_t m_y0;             /**< 頂点0 Y座標 */
      int16_t m_x1;             /**< 頂点1 X座標 */
      int16_t m_y1;             /**< 頂点1 Y座標 */
      int16_t m_x2;             /**< 頂点2 X座標 */
      int16_t m_y2;             /**< 頂点2 Y座標 */
      PixelState m_pixelState;  /**< 点灯状態 */
      bool m_filled;            /**< 塗りつぶし有無 */

      /**
       * @brief Bresenhamアルゴリズムで直線を描画するヘルパー
       *
       * @param fb          描画先フレームバッファ
       * @param x0          始点X座標
       * @param y0          始点Y座標
       * @param x1          終点X座標
       * @param y1          終点Y座標
       * @param pixelState  点灯状態
       */
      static void drawLine(FrameBuffer &fb, int16_t x0, int16_t y0, int16_t x1, int16_t y1, PixelState pixelState);

      /**
       * @brief スキャンラインのX座標を線形補間するヘルパー
       *
       * @param x0 始点X座標
       * @param y0 始点Y座標
       * @param x1 終点X座標
       * @param y1 終点Y座標
       * @param y  補間するY座標
       * @return 補間されたX座標
       */
      static int16_t lerpX(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t y);
  };

} // namespace BinaryGFX

#endif /* USERLIB_BINARYGFX_CORE_INC_OBJECTS_TRIANGLEOBJECT_HPP_ */
