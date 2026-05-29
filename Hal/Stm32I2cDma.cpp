/*
 * Stm32I2cDma.cpp
 *
 *      Author: picman
 */
#include <climits>
#include "../Inc/Stm32I2cDma.hpp"
#include "../Helper/Inc/I2cHelper.hpp"

namespace BinaryGFX::Hal {

  Stm32I2cDma::Stm32I2cDma(I2C_HandleTypeDef *hi2c, uint32_t timeout) :
      m_hi2c(hi2c), m_timeout(timeout) {
  }

  ErrorCode Stm32I2cDma::write(uint8_t deviceAddr, const uint8_t *data, size_t size) {
    if(size > UINT16_MAX) {
      return ErrorCode::InvalidArgument;
    }
    // 7ビットアドレスを8ビットへ変換（HALはR/Wビットを内部で設定する）
    const uint16_t addr8 = static_cast<uint16_t>(deviceAddr << 1);

    // DMA転送中だったら待機
    while (HAL_I2C_GetState(m_hi2c) != HAL_I2C_STATE_READY)
    {
      ;
    }

    // DMA転送中に呼び出し元バッファが無効になるのを防ぐため内部バッファへコピー
    m_dmaBuf.assign(data, data + size);

    const HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_DMA(m_hi2c, addr8, m_dmaBuf.data(), static_cast<uint16_t>(size));
    return mapI2cStatus(status, m_hi2c);
  }

  ErrorCode Stm32I2cDma::read(uint8_t deviceAddr, uint8_t *data, size_t size) {
    if(size > UINT16_MAX) {
      return ErrorCode::InvalidArgument;
    }
    // 7ビットアドレスを8ビットへ変換（HALはR/Wビットを内部で設定する）
    const uint16_t addr8 = static_cast<uint16_t>(deviceAddr << 1);
    // DAM転送中だったら待機
    while (HAL_I2C_GetState(m_hi2c) != HAL_I2C_STATE_READY)
    {
      ;
    }
    const HAL_StatusTypeDef status = HAL_I2C_Master_Receive(m_hi2c, addr8, data, static_cast<uint16_t>(size), m_timeout);
    return mapI2cStatus(status, m_hi2c);
  }

} // namespace BinaryGFX::Hal
