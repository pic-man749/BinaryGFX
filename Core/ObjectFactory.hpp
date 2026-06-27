/*
 * ObjectFactory.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTFACTORY_HPP_
#define BINARYGFX_CORE_OBJECTFACTORY_HPP_

#include <memory>
#include <string>
#include "BinaryGFXCore.hpp"
#include "ObjectId.hpp"
#include "../Common/PixelState.hpp"
#include "Font/BgfxFont_Ascii.hpp"
#include "Binary/BinaryData.hpp"
#include "Objects/PointObject.hpp"
#include "Objects/LineObject.hpp"
#include "Objects/RectangleObject.hpp"
#include "Objects/CircleObject.hpp"
#include "Objects/TriangleObject.hpp"
#include "Objects/TextObject.hpp"
#include "Objects/StringObject.hpp"
#include "Objects/BinaryObject.hpp"

namespace BinaryGFX {

  /**
   * @brief 点オブジェクトを生成し追加する
   *
   * @param gfx        追加先のBinaryGFXインスタンス
   * @param x          X座標
   * @param y          Y座標
   * @param pixelState 点灯状態（デフォルト: 点灯）
   * @param z          Z値（デフォルト: 0）
   * @return TypedObjectId<PointObject> 採番されたオブジェクトID
   */
  inline TypedObjectId<PointObject> createPoint(BinaryGFX &gfx, int16_t x, int16_t y,
                                                PixelState pixelState = PixelState::On,
                                                int16_t z = 0) {
    return gfx.addObject(std::make_unique<PointObject>(x, y, pixelState, z));
  }

  /**
   * @brief 線オブジェクトを生成し追加する
   *
   * @param gfx        追加先のBinaryGFXインスタンス
   * @param x0         始点X座標
   * @param y0         始点Y座標
   * @param x1         終点X座標
   * @param y1         終点Y座標
   * @param pixelState 点灯状態（デフォルト: 点灯）
   * @param z          Z値（デフォルト: 0）
   * @return TypedObjectId<LineObject> 採番されたオブジェクトID
   */
  inline TypedObjectId<LineObject> createLine(BinaryGFX &gfx, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                              PixelState pixelState = PixelState::On,
                                              int16_t z = 0) {
    return gfx.addObject(std::make_unique<LineObject>(x0, y0, x1, y1, pixelState, z));
  }

  /**
   * @brief 矩形オブジェクトを生成し追加する
   *
   * @param gfx        追加先のBinaryGFXインスタンス
   * @param x          左上X座標
   * @param y          左上Y座標
   * @param w          幅（ピクセル）
   * @param h          高さ（ピクセル）
   * @param pixelState 点灯状態（デフォルト: 点灯）
   * @param filled     塗りつぶし有無（デフォルト: 枠線のみ）
   * @param z          Z値（デフォルト: 0）
   * @return TypedObjectId<RectangleObject> 採番されたオブジェクトID
   */
  inline TypedObjectId<RectangleObject> createRectangle(BinaryGFX &gfx, int16_t x, int16_t y, int16_t w, int16_t h,
                                                        PixelState pixelState = PixelState::On,
                                                        bool filled = false, int16_t z = 0) {
    return gfx.addObject(std::make_unique<RectangleObject>(x, y, w, h, pixelState, filled, z));
  }

  /**
   * @brief 円オブジェクトを生成し追加する
   *
   * @param gfx        追加先のBinaryGFXインスタンス
   * @param cx         中心X座標
   * @param cy         中心Y座標
   * @param r          半径（ピクセル）
   * @param pixelState 点灯状態（デフォルト: 点灯）
   * @param filled     塗りつぶし有無（デフォルト: 枠線のみ）
   * @param z          Z値（デフォルト: 0）
   * @return TypedObjectId<CircleObject> 採番されたオブジェクトID
   */
  inline TypedObjectId<CircleObject> createCircle(BinaryGFX &gfx, int16_t cx, int16_t cy, int16_t r,
                                                  PixelState pixelState = PixelState::On,
                                                  bool filled = false, int16_t z = 0) {
    return gfx.addObject(std::make_unique<CircleObject>(cx, cy, r, pixelState, filled, z));
  }

  /**
   * @brief 三角形オブジェクトを生成し追加する
   *
   * @param gfx        追加先のBinaryGFXインスタンス
   * @param x0         頂点0 X座標
   * @param y0         頂点0 Y座標
   * @param x1         頂点1 X座標
   * @param y1         頂点1 Y座標
   * @param x2         頂点2 X座標
   * @param y2         頂点2 Y座標
   * @param pixelState 点灯状態（デフォルト: 点灯）
   * @param filled     塗りつぶし有無（デフォルト: 枠線のみ）
   * @param z          Z値（デフォルト: 0）
   * @return TypedObjectId<TriangleObject> 採番されたオブジェクトID
   */
  inline TypedObjectId<TriangleObject> createTriangle(BinaryGFX &gfx, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                                      int16_t x2,
                                                      int16_t y2, PixelState pixelState = PixelState::On,
                                                      bool filled = false,
                                                      int16_t z = 0) {
    return gfx.addObject(std::make_unique<TriangleObject>(x0, y0, x1, y1, x2, y2, pixelState, filled, z));
  }

  /**
   * @brief テキストオブジェクトを生成し追加する
   *
   * @param gfx        追加先のBinaryGFXインスタンス
   * @param x          描画開始X座標（左上基準）
   * @param y          描画開始Y座標（左上基準）
   * @param text       表示文字列（所有権なし）
   * @param font       使用フォント（所有権なし）
   * @param pixelState 描画色（デフォルト: 点灯）
   * @param z          Z値（デフォルト: 0）
   * @return TypedObjectId<TextObject> 採番されたオブジェクトID
   */
  inline TypedObjectId<TextObject> createText(BinaryGFX &gfx, int16_t x, int16_t y, const char *text,
                                              const FontData *font = &BgfxFont_Ascii,
                                              PixelState pixelState = PixelState::On,
                                              int16_t z = 0) {
    return gfx.addObject(std::make_unique<TextObject>(x, y, text, font, pixelState, z));
  }

  /**
   * @brief Stringオブジェクトを生成し追加する
   *
   * @param gfx        追加先のBinaryGFXインスタンス
   * @param x          描画開始X座標（左上基準）
   * @param y          描画開始Y座標（左上基準）
   * @param string     表示文字列
   * @param font       使用フォント（所有権なし）
   * @param pixelState 描画色（デフォルト: 点灯）
   * @param z          Z値（デフォルト: 0）
   * @return TypedObjectId<StringObject> 採番されたオブジェクトID
   */
  inline TypedObjectId<StringObject> createString(BinaryGFX &gfx, int16_t x, int16_t y, std::string string,
                                                  const FontData *font = &BgfxFont_Ascii,
                                                  PixelState pixelState = PixelState::On, int16_t z = 0) {
    return gfx.addObject(std::make_unique<StringObject>(x, y, std::move(string), font, pixelState, z));
  }

  /**
   * @brief バイナリ画像オブジェクトを生成し追加する
   *
   * @param gfx        追加先のBinaryGFXインスタンス
   * @param x          描画開始X座標（左上基準）
   * @param y          描画開始Y座標（左上基準）
   * @param data       バイナリ画像データ（所有権なし）
   * @param pixelState 描画色（デフォルト: 点灯）
   * @param z          Z値（デフォルト: 0）
   * @return TypedObjectId<BinaryObject> 採番されたオブジェクトID
   */
  inline TypedObjectId<BinaryObject> createBinary(BinaryGFX &gfx, int16_t x, int16_t y, const BinaryData *data,
                                                  PixelState pixelState = PixelState::On, int16_t z = 0) {
    return gfx.addObject(std::make_unique<BinaryObject>(x, y, data, pixelState, z));
  }

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTFACTORY_HPP_ */
