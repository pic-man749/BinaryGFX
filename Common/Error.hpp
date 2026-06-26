/*
 * Error.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_COMMON_ERROR_HPP_
#define BINARYGFX_COMMON_ERROR_HPP_

#include <cstdint>

namespace BinaryGFX {

  /**
   * @brief 各層で発生するエラーの種別
   *
   */
  enum class ErrorCode : uint8_t {
    // @formatter:off
    Ok          = 0,  /**< Ok 成功 */
    Timeout,          /**< Timeout 通信タイムアウト */
    Nack,             /**< Nack I2C NACKの受信 */
    BusError,         /**< BusError バスエラー */
    NotInitialized,   /**< NotInitialized 初期化未完了 */
    InvalidArgument,  /**< InvalidArgument 不正な引数 */
    // @formatter:on
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_COMMON_ERROR_HPP_ */
