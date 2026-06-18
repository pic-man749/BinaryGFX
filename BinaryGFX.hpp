//
// BinaryGFX
//
// SPDX-FileCopyrightText: 2026 PICMAN <me@picman.dev>
// SPDX-License-Identifier: MIT

#ifndef BINARYGFX_BINARYGFX_HPP_
#define BINARYGFX_BINARYGFX_HPP_

// 共通型（ErrorCode, ObjectId, PixelState）
#include "Common/Error.hpp"
#include "Common/PixelState.hpp"

// コア
#include "Core/BinaryGFXCore.hpp"

// フォント
#include "Core/Font/FontData.hpp"

// グラフィックオブジェクト
#include "Core/Objects/IGraphicsObject.hpp"
#include "Core/Objects/PointObject.hpp"
#include "Core/Objects/LineObject.hpp"
#include "Core/Objects/RectangleObject.hpp"
#include "Core/Objects/CircleObject.hpp"
#include "Core/Objects/TriangleObject.hpp"
#include "Core/Objects/TextObject.hpp"
#include "Core/Objects/StringObject.hpp"

// フォント
#include "Core/Font/BgfxFont_Ascii.hpp"

// ドライバ
#include "Driver/Ssd1306Driver.hpp"

// HAL（共通インタフェース）
#include "Hal/ICommInterface.hpp"

// STM32 HAL実装（BGFX_USE_STM32 定義時のみ有効）
#ifdef BGFX_USE_STM32
#include "Hal/Stm32I2c.hpp"
#include "Hal/Stm32I2cDma.hpp"
#endif

#endif /* BINARYGFX_BINARYGFX_HPP_ */
