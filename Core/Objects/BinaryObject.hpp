/*
 * BinaryObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_BINARYOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_BINARYOBJECT_HPP_

#include <cstdint>
#include "../IGraphicsObject.hpp"
#include "../../Common/PixelState.hpp"
#include "../Binary/BinaryData.hpp"

namespace BinaryGFX {

  /**
   * @brief バイナリ画像オブジェクト
   *
   * ユーザが用意した BinaryData を参照し、ビットマップ画像をフレームバッファへ描画する。
   * データポインタの所有権は持たない（ライフタイムはユーザが管理）。
   */
  class BinaryObject: public IGraphicsObject {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param x          描画開始X座標（左上基準）
       * @param y          描画開始Y座標（左上基準）
       * @param data       バイナリ画像データ（所有権なし）
       * @param pixelState 描画色（デフォルト: 点灯）
       * @param z          Z値（デフォルト: 0）
       */
      BinaryObject(int16_t x, int16_t y, const BinaryData *data,
          PixelState pixelState = PixelState::On, int16_t z = 0);

      /**
       * @brief フレームバッファへ自身を描画する
       *
       * @param fb 描画先フレームバッファ
       */
      void render(FrameBuffer &fb) const override;

      /**
       * @brief 描画開始座標を設定する
       *
       * @param x X座標
       * @param y Y座標
       */
      void setPosition(int16_t x, int16_t y);

      /**
       * @brief バイナリ画像データを設定する
       *
       * @param data バイナリ画像データ（所有権なし）
       */
      void setBinaryData(const BinaryData *data);

      /**
       * @brief 描画色を設定する
       *
       * @param pixelState 描画色
       */
      void setPixelState(PixelState pixelState);

    private:

      int16_t m_x;              /**< 描画開始X座標 */
      int16_t m_y;              /**< 描画開始Y座標 */
      const BinaryData *m_data; /**< バイナリ画像データポインタ */
      PixelState m_pixelState;  /**< 描画色 */
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_BINARYOBJECT_HPP_ */
