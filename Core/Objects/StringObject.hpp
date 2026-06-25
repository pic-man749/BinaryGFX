/*
 * StringObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_STRINGOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_STRINGOBJECT_HPP_

#include <cstdint>
#include <string>
#include "../IGraphicsObject.hpp"
#include "../../Common/PixelState.hpp"
#include "../Font/FontData.hpp"

namespace BinaryGFX {

  /**
   * @brief Stringオブジェクト
   *
   * ユーザが用意した FontData を参照し、テキスト文字列をフレームバッファへ描画する。
   * フォントデータの所有権は持たない（ライフタイムはユーザが管理）が、文字列は std::string で保持する。
   */
  class StringObject: public IGraphicsObject {
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
       * @param string 表示文字列
       */
      void setText(const std::string string);

      /**
       * @brief 表示文字列を取得する
       *
       */
      std::string& getText();

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

      int16_t m_x; /**< 描画開始X座標 */
      int16_t m_y; /**< 描画開始Y座標 */
      std::string m_string; /**< 表示文字列 */
      const FontData *m_font; /**< フォントデータポインタ */
      PixelState m_pixelState; /**< 描画色 */
      uint8_t m_charSpacing; /**< 文字間スペース（ピクセル） */
      uint8_t m_lineSpacing; /**< 行間スペース（ピクセル） */
      bool m_wordWrap; /**< ワードラップ有効フラグ */
      uint8_t m_pagesPerCol; /**< グリフ1列あたりのページ数（= ceil(glyphHeight / 8)） */
      uint16_t m_bytesPerGlyph; /**< グリフ1文字のデータサイズ（バイト） */

      /**
       * @brief フォントメトリクスをキャッシュする
       *
       * コンストラクタおよび setFont() から呼び出す。
       */
      void updateFontMetrics();

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

#endif /* BINARYGFX_CORE_OBJECTS_STRINGOBJECT_HPP_ */
