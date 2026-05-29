/*
 * TextObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_TEXTOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_TEXTOBJECT_HPP_

#include <cstdint>
#include "IGraphicsObject.hpp"
#include "../../Common/PixelState.hpp"
#include "../Font/FontData.hpp"

namespace BinaryGFX {

  /**
   * @brief テキストオブジェクト
   *
   * ユーザが用意した FontData を参照し、テキスト文字列をフレームバッファへ描画する。
   * フォントデータおよび文字列ポインタの所有権は持たない（ライフタイムはユーザが管理）。
   */
  class TextObject: public IGraphicsObject {
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
      TextObject(int16_t x, int16_t y, const char *text, const FontData *font,
                 PixelState pixelState = PixelState::On, int16_t z = 0);

      /**
       * @brief フレームバッファへ自身を描画する
       *
       * @param fb 描画先フレームバッファ
       */
      void render(FrameBuffer &fb) const override;

      /**
       * @brief 描画開始座標を設定する
       *
       * @param x X座標
       * @param y Y座標
       */
      void setPosition(int16_t x, int16_t y);

      /**
       * @brief 表示文字列を設定する
       *
       * @param text 表示文字列（所有権なし）
       */
      void setText(const char *text);

      /**
       * @brief 使用フォントを設定する
       *
       * @param font 使用フォント（所有権なし）
       */
      void setFont(const FontData *font);

      /**
       * @brief 描画色を設定する
       *
       * @param pixelState 描画色
       */
      void setPixelState(PixelState pixelState);

      /**
       * @brief 文字間スペースを設定する
       *
       * @param spacing 文字間スペース（ピクセル）
       */
      void setCharSpacing(uint8_t spacing);

      /**
       * @brief 行間スペースを設定する
       *
       * @param spacing 行間スペース（ピクセル）
       */
      void setLineSpacing(uint8_t spacing);

      /**
       * @brief ワードラップの有効・無効を設定する
       *
       * @param enable true:有効、false:無効
       */
      void setWordWrap(bool enable);

    private:

      int16_t         m_x;           /**< 描画開始X座標 */
      int16_t         m_y;           /**< 描画開始Y座標 */
      const char     *m_text;        /**< 表示文字列ポインタ */
      const FontData *m_font;        /**< フォントデータポインタ */
      PixelState      m_pixelState;  /**< 描画色 */
      uint8_t         m_charSpacing; /**< 文字間スペース（ピクセル） */
      uint8_t         m_lineSpacing; /**< 行間スペース（ピクセル） */
      bool            m_wordWrap;    /**< ワードラップ有効フラグ */

      /**
       * @brief 1グリフをフレームバッファへ描画する
       *
       * @param fb 描画先フレームバッファ
       * @param x  グリフ描画X座標（左上基準）
       * @param y  グリフ描画Y座標（左上基準）
       * @param c  文字コード
       */
      void drawGlyph(FrameBuffer &fb, int16_t x, int16_t y, uint8_t c) const;
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_TEXTOBJECT_HPP_ */
