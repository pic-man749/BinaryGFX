/*
 * TextObjectBase.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_TEXTOBJECTBASE_HPP_
#define BINARYGFX_CORE_OBJECTS_TEXTOBJECTBASE_HPP_

#include <cstdint>
#include "../IGraphicsObject.hpp"
#include "../../Common/PixelState.hpp"
#include "../Font/FontData.hpp"

namespace BinaryGFX {

  /**
   * @brief テキストの水平方向の揃え
   */
  enum class TextAlign : uint8_t {
    Left,   /**< 左揃え（デフォルト） */
    Center, /**< 中央揃え */
    Right,  /**< 右揃え */
  };

  /**
   * @brief TextObject / StringObject の共通基底クラス
   *
   * ユーザが用意した FontData を参照し、テキスト文字列をフレームバッファへ描画する
   * 共通ロジック（位置・フォント・色・字間/行間スペース・ワードラップ・揃え・描画処理）を提供する。
   * 文字列の保持方法（所有権あり/なし）は派生クラスが textPtr() を実装して提供する。
   */
  class TextObjectBase: public IGraphicsObject {
    public:

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

      /**
       * @brief 揃え・ワードラップの基準幅を設定する
       *
       * @param width 基準幅（ピクセル）。0を指定するとx座標からフレームバッファ右端までを自動的に使用する（デフォルト: 0）。
       */
      void setWidth(uint16_t width);

      /**
       * @brief 水平方向の揃えを設定する
       *
       * @param align 揃え（デフォルト: Left）
       */
      void setAlign(TextAlign align);

    protected:

      /**
       * @brief コンストラクタ
       *
       * @param x          描画開始X座標（左上基準）
       * @param y          描画開始Y座標（左上基準）
       * @param font       使用フォント（所有権なし）
       * @param pixelState 描画色
       * @param z          Z値
       */
      TextObjectBase(int16_t x, int16_t y, const FontData *font, PixelState pixelState, int16_t z);

      /**
       * @brief 表示文字列（NUL終端）へのポインタを返す
       *
       * 文字列が未設定の場合は nullptr を返してよい。
       *
       * @return 表示文字列ポインタ
       */
      virtual const char* textPtr() const = 0;

    private:

      int16_t m_x;              /**< 描画開始X座標 */
      int16_t m_y;              /**< 描画開始Y座標 */
      const FontData *m_font;   /**< フォントデータポインタ */
      PixelState m_pixelState;  /**< 描画色 */
      uint8_t m_charSpacing;    /**< 文字間スペース（ピクセル） */
      uint8_t m_lineSpacing;    /**< 行間スペース（ピクセル） */
      bool m_wordWrap;          /**< ワードラップ有効フラグ */
      uint16_t m_width;         /**< 揃え・ワードラップの基準幅（0:自動） */
      TextAlign m_align;        /**< 水平方向の揃え */
      uint8_t m_pagesPerCol;    /**< グリフ1列あたりのページ数（= ceil(glyphHeight / 8)） */
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

      /**
       * @brief 1行分の終端位置・次行開始位置・表示幅を求める（計測のみ、描画は行わない）
       *
       * ワードラップが有効な場合、常に左揃え相当の位置（boxLeft起点）で折り返し判定を行う。
       * 揃え方向による開始X座標の違いは、この行分割の結果には影響しない。
       * ボックス幅が1文字分に満たない場合でも、行の先頭文字は必ず配置し（強制的に1文字は前進させ）、
       * 無限ループを防ぐ。
       *
       * @param lineStart 走査開始位置（行の先頭）
       * @param boxLeft   ボックス左端のX座標
       * @param boxRight  ボックス右端のX座標
       * @param lineEnd   [out] 行の終端位置（この位置の文字は含まない）
       * @param nextStart [out] 次行の開始位置
       * @return 行の表示幅（ピクセル、末尾の字間スペースは含まない）
       */
      int32_t findLineEnd(const char *lineStart, int32_t boxLeft, int32_t boxRight,
                          const char **lineEnd,
                          const char **nextStart) const;

      /**
       * @brief 計測済みの1行を指定座標へ描画する
       *
       * @param fb    描画先フレームバッファ
       * @param start 行の開始位置
       * @param end   行の終端位置（この位置の文字は含まない）
       * @param x     描画開始X座標
       * @param y     描画開始Y座標
       */
      void drawLineChars(FrameBuffer &fb, const char *start, const char *end, int16_t x, int16_t y) const;

      /**
       * @brief 揃え設定に応じた行の開始X座標を計算する
       *
       * @param boxLeft   ボックス左端のX座標
       * @param boxWidth  ボックス幅
       * @param lineWidth 行の表示幅
       * @return 開始X座標
       */
      int16_t computeAlignedX(int32_t boxLeft, int32_t boxWidth, int32_t lineWidth) const;
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_TEXTOBJECTBASE_HPP_ */
