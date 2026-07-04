/*
 * FactoryStm32I2c.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_GFXFACTORY_FACTORYSTM32I2C_HPP_
#define BINARYGFX_CORE_GFXFACTORY_FACTORYSTM32I2C_HPP_

#include <memory>
#include "../BinaryGFXCore.hpp"
#include "../../Driver/Ssd1306Driver.hpp"
#include "../../Hal/Stm32I2c.hpp"
#include "../../Hal/Stm32TickProvider.hpp"

namespace BinaryGFX {

  /**
   * @brief STM32 I2C（ポーリング）接続のSSD1306構成でBinaryGFXを生成する
   *
   * Stm32I2c・Ssd1306Driver・Stm32TickProviderの生成と依存注入を一括して行う。
   * 生成されたインスタンスのinit()呼び出しは呼び出し側の責務とする。
   *
   * @param hi2c       使用するI2Cハンドル
   * @param deviceAddr SSD1306デバイスアドレス（7ビット値）
   * @param width      ディスプレイ幅（ピクセル）
   * @param height     ディスプレイ高さ（ピクセル）
   * @return std::unique_ptr<BinaryGFX> 生成されたBinaryGFXインスタンス
   */
  inline std::unique_ptr<BinaryGFX> createGfxStm32I2c(I2C_HandleTypeDef *hi2c,
                                                      uint8_t deviceAddr,
                                                      uint16_t width, uint16_t height) {
    auto comm = std::make_unique<Hal::Stm32I2c>(hi2c, 1000);
    auto driver = std::make_unique<Driver::Ssd1306Driver>(std::move(comm), deviceAddr, width, height);
    auto tickProvider = std::make_unique<Hal::Stm32TickProvider>();
    return std::make_unique<BinaryGFX>(std::move(driver), std::move(tickProvider));
  }

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_GFXFACTORY_FACTORYSTM32I2C_HPP_ */
