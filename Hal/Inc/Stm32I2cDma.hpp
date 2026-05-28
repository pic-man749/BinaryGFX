/*
 * Stm32I2cDma.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_HAL_INC_STM32I2CDMA_HPP_
#define BINARYGFX_HAL_INC_STM32I2CDMA_HPP_

#include "ICommInterface.hpp"
#include "stm32f4xx_hal.h"
#include <vector>

namespace BinaryGFX::Hal {

  /**
   * @brief STM32 HAL I2C ラッパークラス
   *
   * STM32 HAL の I2C マスター転送・受信を ICommInterface にラップする。
   * デバイスアドレスは 7 ビット値を受け取り、内部で 8 ビットへ変換する。
   */
  class Stm32I2cDma: public ICommInterface {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param hi2c    使用する I2C ハンドル
       * @param timeout 通信タイムアウト値（ミリ秒）
       */
      Stm32I2cDma(I2C_HandleTypeDef *hi2c, uint32_t timeout);

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

      /**
       * @brief HAL_StatusTypeDef を ErrorCode にマッピングする
       *
       * HAL_ERROR 時は HAL_I2C_GetError() を参照し、NACK か BusError かを判定する。
       *
       * @param status HAL 関数の戻り値
       * @return ErrorCode マッピング後のエラーコード
       */
      ErrorCode mapStatus(HAL_StatusTypeDef status) const;

      I2C_HandleTypeDef *m_hi2c; /**< I2C ハンドル */
      uint32_t m_timeout; /**< 通信タイムアウト値（ミリ秒） */
      std::vector<uint8_t> m_dmaBuf; /**< DMA 転送用バッファ */
  };

} // namespace BinaryGFX::Hal

#endif /* BINARYGFX_HAL_INC_STM32I2CDMA_HPP_ */
