/*
 * Stm32TickProvider.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_HAL_STM32TICKPROVIDER_HPP_
#define BINARYGFX_HAL_STM32TICKPROVIDER_HPP_

#include "../Core/ITickProvider.hpp"

namespace BinaryGFX::Hal {

  class Stm32TickProvider: public ITickProvider {

    public:
      uint32_t getTickMs() const override;
  };

} // namespace BinaryGFX::Hal

#endif /* BINARYGFX_HAL_STM32TICKPROVIDER_HPP_ */
