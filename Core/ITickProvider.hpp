/*
 * ITickProvider.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_ITICKPROVIDER_HPP_
#define BINARYGFX_CORE_ITICKPROVIDER_HPP_

#include <cstdint>

namespace BinaryGFX {

  /**
   * @brief Tick取得インタフェース
   *
   * BinaryGFXがアニメーション等の時間管理に使用するTickを取得する。
   * 単位はミリ秒。
   */
  class ITickProvider {
    public:

      virtual ~ITickProvider() = default;

      /**
       * @brief 現在のTick値を取得する
       *
       * @return uint32_t 現在のTick値（ミリ秒、オーバーフロー時は0へ周回）
       */
      virtual uint32_t getTickMs() const = 0;
  };

} // namespace BinaryGFX::Hal

#endif /* BINARYGFX_CORE_ITICKPROVIDER_HPP_ */
