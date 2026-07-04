/*
 * TextObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_TEXTOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_TEXTOBJECT_HPP_

#include <cstdint>
#include "TextObjectBase.hpp"
#include "../../Common/PixelState.hpp"
#include "../Font/FontData.hpp"

namespace BinaryGFX {

  /**
   * @brief テキストオブジェクト
   *
   * ユーザが用意した FontData を参照し、テキスト文字列をフレームバッファへ描画する。
   * フォントデータおよび文字列ポインタの所有権は持たない（ライフタイムはユーザが管理）。
   * 位置・フォント・色・字間/行間スペース・ワードラップ・幅・揃えの設定は TextObjectBase が提供する。
   */
  class TextObject: public TextObjectBase {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param x          描画開始X座標（左上基準）
       * @param y          描画開始Y座標（左上基準）
       * @param text       表示文字列（所有権なし）
       * @param font       使用フォント（所有権なし）
       * @param pixelState 描画色（デフォルト: 点灯）
       * @param z          Z値（デフォルト: 0）
       */
      TextObject(int16_t x, int16_t y, const char *text, const FontData *font, PixelState pixelState = PixelState::On, int16_t z = 0);

      /**
       * @brief 表示文字列を設定する
       *
       * @param text 表示文字列（所有権なし）
       */
      void setText(const char *text);

    protected:

      const char* textPtr() const override;

    private:

      const char *m_text; /**< 表示文字列ポインタ */
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_TEXTOBJECT_HPP_ */
