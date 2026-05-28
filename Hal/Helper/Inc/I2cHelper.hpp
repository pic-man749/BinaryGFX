/*
 * I2cHelper.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_HAL_HELPER_INC_I2CHELPER_HPP_
#define BINARYGFX_HAL_HELPER_INC_I2CHELPER_HPP_

#include "../Inc/I2cHelper.hpp"
#include "../../../Common/Inc/Error.hpp"
#include "i2c.h"

namespace BinaryGFX::Hal {

  /**
   * @brief HAL_StatusTypeDef を ErrorCode にマッピングする
   *
   * HAL_ERROR 時は HAL_I2C_GetError() を参照し、NACK か BusError かを判定する。
   *
   * @param status HAL 関数の戻り値
   * @param hi2c   エラー詳細取得に使用する I2C ハンドル
   * @return ErrorCode マッピング後のエラーコード
   */
  ErrorCode mapI2cStatus(HAL_StatusTypeDef status, I2C_HandleTypeDef *hi2c);

} // namespace BinaryGFX::Hal

#endif /* BINARYGFX_HAL_HELPER_INC_I2CHELPER_HPP_ */
