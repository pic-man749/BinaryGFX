/*
 * I2cHelper.cpp
 *
 *      Author: picman
 */

#include "../Inc/I2cHelper.hpp"

namespace BinaryGFX::Hal {

  ErrorCode mapI2cStatus(HAL_StatusTypeDef status, I2C_HandleTypeDef *hi2c) {
    switch(status) {
      case HAL_OK:
        return ErrorCode::Ok;
      case HAL_TIMEOUT:
        return ErrorCode::Timeout;
      case HAL_ERROR:
        // HAL_I2C_ERROR_AF（Acknowledge Failure）はNACKを示す
        if((HAL_I2C_GetError(hi2c) & HAL_I2C_ERROR_AF) != 0u) {
          return ErrorCode::Nack;
        }
        return ErrorCode::BusError;
      case HAL_BUSY:
      default:
        return ErrorCode::BusError;
    }
  }

} // namespace BinaryGFX::Hal
