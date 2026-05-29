/*
 * PixelState.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_COMMON_PIXELSTATE_HPP_
#define BINARYGFX_COMMON_PIXELSTATE_HPP_

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

#endif /* BINARYGFX_COMMON_PIXELSTATE_HPP_ */
