/*
 * Stm32TickProvider.cpp
 *
 *      Author: picman
 */
#include "Stm32TickProvider.hpp"
#include "main.h"

namespace BinaryGFX::Hal {

  uint32_t Stm32TickProvider::getTickMs() const {
    return HAL_GetTick();
  }

}
