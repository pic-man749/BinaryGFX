/*
 * FrameBuffer.hpp
 *
 *      Author: picman
 */

#ifndef USERLIB_BINARYGFX_CORE_INC_FRAMEBUFFER_HPP_
#define USERLIB_BINARYGFX_CORE_INC_FRAMEBUFFER_HPP_

#include <cstdint>
#include <cstddef>
#include <vector>
#include "../../Common/Inc/PixelState.hpp"

namespace BinaryGFX {

  /**
   * @brief フレームバッファ
   *
   * SSD1306縦ページ形式でピクセルデータを保持する。
   * pixel(x, y) → m_buffer[(y/8) * width + x] の bit(y%8)
   * バッファサイズ: width * (height / 8) バイト
   */
  class FrameBuffer {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param width  ディスプレイ幅（ピクセル）
       * @param height ディスプレイ高さ（ピクセル、8の倍数）
       */
      FrameBuffer(uint16_t width, uint16_t height);

      /**
       * @brief 指定座標のピクセルを設定する
       *
       * 範囲外の座標は無視する。
       *
       * @param x           X座標
       * @param y           Y座標
       * @param pixelState  点灯状態
       */
      void setPixel(int16_t x, int16_t y, PixelState pixelState);

      /**
       * @brief 指定座標のピクセル状態を取得する
       *
       * 範囲外の座標はfalseを返す。
       *
       * @param x X座標
       * @param y Y座標
       * @return true:点灯、false:消灯
       */
      bool getPixel(int16_t x, int16_t y) const;

      /**
       * @brief フレームバッファ全体を塗りつぶす
       *
       * @param pixelState 塗りつぶす点灯状態（デフォルト: 消灯）
       */
      void clear(PixelState pixelState = PixelState::Off);

      /**
       * @brief フレームバッファの先頭ポインタを返す
       *
       * @return バッファデータへのポインタ
       */
      const uint8_t* getData() const;

      /**
       * @brief フレームバッファのサイズを返す
       *
       * @return バッファサイズ（バイト）
       */
      size_t getDataSize() const;

      /**
       * @brief ディスプレイ幅を返す
       *
       * @return ディスプレイ幅（ピクセル）
       */
      uint16_t getWidth() const;

      /**
       * @brief ディスプレイ高さを返す
       *
       * @return ディスプレイ高さ（ピクセル）
       */
      uint16_t getHeight() const;

    private:

      uint16_t m_width; /**< ディスプレイ幅 */
      uint16_t m_height; /**< ディスプレイ高さ */
      std::vector<uint8_t> m_buffer; /**< ピクセルデータバッファ */
  };

} // namespace BinaryGFX

#endif /* USERLIB_BINARYGFX_CORE_INC_FRAMEBUFFER_HPP_ */
