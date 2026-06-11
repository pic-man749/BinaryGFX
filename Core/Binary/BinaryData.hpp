/*
 * BinaryData.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_BINARY_BINARYDATA_HPP_
#define BINARYGFX_CORE_BINARY_BINARYDATA_HPP_

#include <cstdint>

namespace BinaryGFX {

  /**
   * @brief バイナリ画像データ
   *
   * 縦ページ形式（column-major）、LSB が上端のビットマップデータを定義する。
   * FontData と同じデータ形式のため、フォント生成ツールと同様の変換ツールが使用できる。
   *
   * データレイアウト:
   *   データサイズ = width × ceil(height / 8) バイト
   *   配置: [col0_page0, col0_page1, ..., col1_page0, col1_page1, ..., colN_pageM]
   *
   * インデックス計算:
   *   pagesPerCol = (height + 7) / 8
   *   byte = data[col * pagesPerCol + page]
   *   pixel(col, row): (byte >> (row % 8)) & 0x01、bit0 が上端
   */
  typedef struct {
      uint16_t       width;  /**< 画像幅（ピクセル） */
      uint16_t       height; /**< 画像高さ（ピクセル） */
      const uint8_t* data;   /**< ビットマップデータ（ROM 配置可）*/
  } BinaryData;

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_BINARY_BINARYDATA_HPP_ */
