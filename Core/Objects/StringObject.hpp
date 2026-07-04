/*
 * StringObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_STRINGOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_STRINGOBJECT_HPP_

#include <cstdint>
#include <string>
#include "TextObjectBase.hpp"
#include "../../Common/PixelState.hpp"
#include "../Font/FontData.hpp"

namespace BinaryGFX {

  /**
   * @brief Stringオブジェクト
   *
   * ユーザが用意した FontData を参照し、テキスト文字列をフレームバッファへ描画する。
   * フォントデータの所有権は持たない（ライフタイムはユーザが管理）が、文字列は std::string で保持する。
   * 位置・フォント・色・字間/行間スペース・ワードラップ・幅・揃えの設定は TextObjectBase が提供する。
   */
  class StringObject: public TextObjectBase {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param x          描画開始X座標（左上基準）
       * @param y          描画開始Y座標（左上基準）
       * @param string     表示文字列
       * @param font       使用フォント（所有権なし）
       * @param pixelState 描画色（デフォルト: 点灯）
       * @param z          Z値（デフォルト: 0）
       */
      StringObject(int16_t x, int16_t y, std::string string, const FontData *font, PixelState pixelState = PixelState::On, int16_t z = 0);

      /**
       * @brief 表示文字列を設定する
       *
       * @param string 表示文字列
       */
      void setText(const std::string string);

      /**
       * @brief 表示文字列を取得する
       *
       */
      std::string& getText();

    protected:

      const char* textPtr() const override;

    private:

      std::string m_string; /**< 表示文字列 */
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_STRINGOBJECT_HPP_ */
