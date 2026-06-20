/*
 * Stm32I2c.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_HAL_STM32I2C_HPP_
#define BINARYGFX_HAL_STM32I2C_HPP_

#include "../Driver/ICommInterface.hpp"
#include "i2c.h"

namespace BinaryGFX::Hal {

  /**
   * @brief STM32 HAL I2C ラッパークラス
   *
   * STM32 HAL の I2C マスター転送・受信を ICommInterface にラップする。
   * デバイスアドレスは 7 ビット値を受け取り、内部で 8 ビットへ変換する。
   */
  class Stm32I2c: public ICommInterface {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param hi2c    使用する I2C ハンドル
       * @param timeout 通信タイムアウト値（ミリ秒）
       */
      Stm32I2c(I2C_HandleTypeDef *hi2c, uint32_t timeout);

      /**
       * @brief データを送信する
       *
       * @param deviceAddr デバイスアドレス（7ビット値）
       * @param data       送信データ
       * @param size       送信データサイズ（バイト）
       * @return ErrorCode 送信結果
       */
      ErrorCode write(uint8_t deviceAddr, const uint8_t *data, size_t size) override;

      /**
       * @brief データを受信する
       *
       * @param deviceAddr デバイスアドレス（7ビット値）
       * @param data       受信バッファ
       * @param size       受信データサイズ（バイト）
       * @return ErrorCode 受信結果
       */
      ErrorCode read(uint8_t deviceAddr, uint8_t *data, size_t size) override;

    private:

      I2C_HandleTypeDef *m_hi2c; /**< I2C ハンドル */
      uint32_t m_timeout; /**< 通信タイムアウト値（ミリ秒） */
  };

} // namespace BinaryGFX::Hal

#endif /* BINARYGFX_HAL_STM32I2C_HPP_ */
