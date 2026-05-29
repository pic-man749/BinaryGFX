/*
 * FontData.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_FONT_FONTDATA_HPP_
#define BINARYGFX_CORE_FONT_FONTDATA_HPP_

#include <cstdint>

namespace BinaryGFX {

  /**
   * @brief ビットマップフォントデータ
   *
   * 縦ページ形式（column-major）、LSB が上端のビットマップフォントを定義する。
   *
   * データレイアウト:
   *   1グリフのデータサイズ = glyphWidth × ceil(glyphHeight / 8) バイト
   *   配置: [col0_page0, col0_page1, ..., col1_page0, col1_page1, ..., colN_pageM]
   *
   * インデックス計算:
   *   pagesPerCol  = (glyphHeight + 7) / 8
   *   glyphOffset  = glyphIndex * (glyphWidth * pagesPerCol)
   *   byte = data[glyphOffset + col * pagesPerCol + page]
   *   pixel(col, row): (byte >> (row % 8)) & 0x01、bit0 が上端
   */
  typedef struct {
      uint8_t        glyphWidth;   /**< グリフ幅（ピクセル） */
      uint8_t        glyphHeight;  /**< グリフ高さ（ピクセル） */
      uint8_t        firstChar;    /**< 先頭文字コード（例: 0x20） */
      uint8_t        charCount;    /**< グリフ数 */
      const uint8_t* data;         /**< グリフビットマップデータ（ROM 配置可） */
  } FontData;

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_FONT_FONTDATA_HPP_ */
