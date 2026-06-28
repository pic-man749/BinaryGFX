/*
 * AnimationFrames.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_BINARY_ANIMATIONFRAMES_HPP_
#define BINARYGFX_CORE_BINARY_ANIMATIONFRAMES_HPP_

#include <cstdint>
#include "BinaryData.hpp"

namespace BinaryGFX {

  struct AnimationFrames{
      const BinaryData* const* frames;  /**< フレーム配列の先頭ポインタ */
      uint16_t frameCount;              /**< フレーム数（2以上であること） */
      uint32_t intervalMs;              /**< フレーム切替間隔（ミリ秒） */
  };
}

#endif /* BINARYGFX_CORE_BINARY_ANIMATIONFRAMES_HPP_ */
