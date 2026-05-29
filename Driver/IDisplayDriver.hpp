/*
 * IDisplayDriver.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_DRIVER_IDISPLAYDRIVER_HPP_
#define BINARYGFX_DRIVER_IDISPLAYDRIVER_HPP_

#include <cstdint>
#include <cstddef>
#include "../Common/Error.hpp"

namespace BinaryGFX::Driver {

  /**
   * @brief ディスプレイドライバのインタフェース
   *
   * 中間層のディスプレイドライバが実装すべきインタフェースを定義する。
   * BinaryGFXコアはこのインタフェースを通じてディスプレイを制御する。
   */
  class IDisplayDriver {
    public:

      virtual ~IDisplayDriver() = default;

      /**
       * @brief ディスプレイを初期化する
       *
       * @return true 初期化成功
       * @return false 初期化失敗
       */
      virtual bool init() = 0;

      /**
       * @brief フレームバッファデータをディスプレイへ転送する
       *
       * データフォーマット: SSD1306縦ページ形式
       * pixel(x,y) → data[(y/8)*width + x] の bit(y%8)
       *
       * @param data 転送するフレームバッファデータ
       * @param size データサイズ（バイト）
       * @return ErrorCode 転送結果
       */
      virtual ErrorCode sendBuffer(const uint8_t *data, size_t size) = 0;

      /**
       * @brief ディスプレイの表示ON/OFFを切り替える
       *
       * @param on true:表示ON、false:表示OFF
       */
      virtual void setDisplayOn(bool on) = 0;

      /**
       * @brief ディスプレイの幅を返す
       *
       * init()呼び出し前から有効。
       *
       * @return ディスプレイ幅（ピクセル）
       */
      virtual uint16_t getWidth() const = 0;

      /**
       * @brief ディスプレイの高さを返す
       *
       * init()呼び出し前から有効。
       *
       * @return ディスプレイ高さ（ピクセル）
       */
      virtual uint16_t getHeight() const = 0;
  };

} // namespace BinaryGFX::Driver

#endif /* BINARYGFX_DRIVER_IDISPLAYDRIVER_HPP_ */
