/*
 * ICommInterface.hpp
 *
 *      Author: picman
 */

#ifndef USERLIB_BINARYGFX_HAL_INC_ICOMMINTERFACE_HPP_
#define USERLIB_BINARYGFX_HAL_INC_ICOMMINTERFACE_HPP_

#include <cstdint>
#include <cstddef>
#include "../../Common/Inc/Error.hpp"

namespace BinaryGFX::Hal {

  /**
   * @brief 通信インタフェースの基底インタフェース
   *
   * 下位層のHALラッパーが実装すべきインタフェースを定義する。
   * ディスプレイドライバはこのインタフェースを通じて通信を行う。
   */
  class ICommInterface {
    public:

      virtual ~ICommInterface() = default;

      /**
       * @brief データを送信する
       *
       * @param deviceAddr デバイスアドレス（7ビット値）
       * @param data 送信データ
       * @param size 送信データサイズ（バイト）
       * @return ErrorCode 送信結果
       */
      virtual ErrorCode write(uint8_t deviceAddr, const uint8_t *data, size_t size) = 0;

      /**
       * @brief データを受信する
       *
       * @param deviceAddr デバイスアドレス（7ビット値）
       * @param data 受信バッファ
       * @param size 受信データサイズ（バイト）
       * @return ErrorCode 受信結果
       */
      virtual ErrorCode read(uint8_t deviceAddr, uint8_t *data, size_t size) = 0;
  };

} // namespace BinaryGFX::Hal

#endif /* USERLIB_BINARYGFX_HAL_INC_ICOMMINTERFACE_HPP_ */
