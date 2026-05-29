/*
 * Error.hpp
 *
 *      Author: picman
 */

#ifndef USERLIB_BINARYGFX_COMMON_INC_ERROR_HPP_
#define USERLIB_BINARYGFX_COMMON_INC_ERROR_HPP_

#include <cstdint>

namespace BinaryGFX {

  /** オブジェクトを一意に識別するID */
  using ObjectId = uint32_t;

  /** 無効なオブジェクトIDを表す定数 */
  static constexpr ObjectId InvalidObjectId = 0u;

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

#endif /* USERLIB_BINARYGFX_COMMON_INC_ERROR_HPP_ */
