/*
 * IGraphicsObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_IGRAPHICSOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_IGRAPHICSOBJECT_HPP_

#include <cstdint>

namespace BinaryGFX {

// FrameBufferはPhase3で実装するため前方宣言
  class FrameBuffer;

  /**
   * @brief グラフィックオブジェクトの基底インタフェース
   *
   * すべての描画オブジェクトはこのクラスを継承する。
   * Z値によって描画順序（重なり）を制御できる。
   * Z値が大きいオブジェクトほど前面（後から描画）に配置される。
   */
  class IGraphicsObject {
    public:

      virtual ~IGraphicsObject() = default;

      /**
       * @brief フレームバッファへ自身を描画する
       *
       * @param fb 描画先フレームバッファ
       */
      virtual void render(FrameBuffer &fb) const = 0;

      /**
       * @brief 時間経過に応じた内部状態を更新する
       *
       * アニメーション等、時間経過に応じて内部状態を変化させるオブジェクトのみが
       * オーバーライドする。デフォルトでは何も行わない。
       *
       * @param tickMs 現在のTick値（ミリ秒）
       */
      virtual void update(uint32_t tickMs) {
        static_cast<void>(tickMs);
      }

      /**
       * @brief Z値を取得する
       *
       * @return Z値
       */
      int16_t getZ() const {
        return m_z;
      }

      /**
       * @brief Z値を設定する
       *
       * @param z Z値（値が大きいほど前面に描画される）
       */
      void setZ(int16_t z) {
        m_z = z;
      }

    protected:

      /**
       * @brief コンストラクタ
       *
       * @param z 初期Z値（デフォルト: 0）
       */
      explicit IGraphicsObject(int16_t z = 0) :
          m_z(z) {
      }

    private:

      int16_t m_z; /**< Z値（描画順序の制御に使用） */
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_IGRAPHICSOBJECT_HPP_ */
