/*
 * Stm32I2c.cpp
 *
 *      Author: picman
 */

#include <climits>
#include "Stm32I2c.hpp"
#include "Helper/I2cHelper.hpp"

namespace BinaryGFX::Hal {

  Stm32I2c::Stm32I2c(I2C_HandleTypeDef *hi2c, uint32_t timeout) :
      m_hi2c(hi2c), m_timeout(timeout) {
  }

  ErrorCode Stm32I2c::write(uint8_t deviceAddr, const uint8_t *data, size_t size) {
    if(size > UINT16_MAX) {
      return ErrorCode::InvalidArgument;
    }
    // 7ビットアドレスを8ビットへ変換（HALはR/Wビットを内部で設定する）
    const uint16_t addr8 = static_cast<uint16_t>(deviceAddr << 1);
    const HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(m_hi2c, addr8, const_cast<uint8_t*>(data), static_cast<uint16_t>(size), m_timeout);
    return mapI2cStatus(status, m_hi2c);
  }

  ErrorCode Stm32I2c::read(uint8_t deviceAddr, uint8_t *data, size_t size) {
    if(size > UINT16_MAX) {
      return ErrorCode::InvalidArgument;
    }
    // 7ビットアドレスを8ビットへ変換（HALはR/Wビットを内部で設定する）
    const uint16_t addr8 = static_cast<uint16_t>(deviceAddr << 1);
    const HAL_StatusTypeDef status = HAL_I2C_Master_Receive(m_hi2c, addr8, data, static_cast<uint16_t>(size), m_timeout);
    return mapI2cStatus(status, m_hi2c);
  }

} // namespace BinaryGFX::Hal
