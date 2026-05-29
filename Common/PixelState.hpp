/*
 * PixelState.hpp
 *
 *      Author: picman
 */

#ifndef USERLIB_BINARYGFX_COMMON_INC_PIXELSTATE_HPP_
#define USERLIB_BINARYGFX_COMMON_INC_PIXELSTATE_HPP_

#include <cstdint>

namespace BinaryGFX {

  /**
   * @brief 2値ディスプレイのピクセル点灯状態
   *
   */
  enum class PixelState : uint8_t {
    // @formatter:off
    Off = 0,  /**< Off 消灯  */
    On = 1,   /**< On 点灯 */
    // @formatter:on
  };

} // namespace BinaryGFX

#endif /* USERLIB_BINARYGFX_COMMON_INC_PIXELSTATE_HPP_ */
