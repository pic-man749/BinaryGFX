/*
 * ObjectId.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTID_HPP_
#define BINARYGFX_CORE_OBJECTID_HPP_

#include <cstdint>

namespace BinaryGFX {

  class BinaryGFX;  // TypedObjectIdへ有効な値を発行できる相手を限定するための前方宣言

  /** オブジェクトを一意に識別する型消去済みのID */
  using ObjectId = uint32_t;

  /** 無効なオブジェクトIDを表す定数 */
  static constexpr ObjectId InvalidObjectId = 0u;

  /**
   * @brief 取得対象オブジェクトの型情報を保持するオブジェクトID
   *
   * テンプレートパラメータTで対象オブジェクトの型を表現することで、
   * getObjectById()呼び出し時に呼び出し側が型を明示する必要をなくし、
   * 取得対象の型取り違えをコンパイル時に防止する。
   * 有効な値を持つインスタンスはBinaryGFX::addObject()内でのみ生成される。
   *
   * @tparam T 識別対象のグラフィックオブジェクト型
   */
  template<typename T>
  class TypedObjectId {

    public:
      /** 無効なIDとして構築する */
      constexpr TypedObjectId() :
          m_value(InvalidObjectId) {
      }

      /**
       * @brief IDが有効かどうかを判定する
       *
       * @return true 有効なIDである, false 無効なIDである
       */
      constexpr bool isValid() const {
        return m_value != InvalidObjectId;
      }

      /**
       * @brief 型情報を消去した生IDへ変換する
       *
       * removeObject()など型を必要としないAPIへ渡す際に使用する。
       */
      constexpr operator ObjectId() const {
        return m_value;
      }

      constexpr bool operator==(const TypedObjectId &other) const {
        return m_value == other.m_value;
      }

      constexpr bool operator!=(const TypedObjectId &other) const {
        return !(*this == other);
      }

    private:
      explicit constexpr TypedObjectId(ObjectId value) :
          m_value(value) {
      }

      ObjectId m_value; /**< 型消去済みの実体ID */

      friend class BinaryGFX; /**< addObject()のみが有効な値を発行できるようにする */
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTID_HPP_ */
