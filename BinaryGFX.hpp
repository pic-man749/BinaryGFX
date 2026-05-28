//
// BinaryGFX
//
// SPDX-FileCopyrightText: 2026 PICMAN <me@picman.dev>
// SPDX-License-Identifier: MIT

#ifndef BINARYGFX_BINARYGFX_HPP_
#define BINARYGFX_BINARYGFX_HPP_

// 共通型（ErrorCode, ObjectId, PixelState）
#include "Common/Inc/Error.hpp"
#include "Common/Inc/PixelState.hpp"

// コア
#include "Core/Inc/BinaryGFXCore.hpp"

// グラフィックオブジェクト
#include "Core/Inc/Objects/IGraphicsObject.hpp"
#include "Core/Inc/Objects/PointObject.hpp"
#include "Core/Inc/Objects/LineObject.hpp"
#include "Core/Inc/Objects/RectangleObject.hpp"
#include "Core/Inc/Objects/CircleObject.hpp"
#include "Core/Inc/Objects/TriangleObject.hpp"

// ドライバ
#include "Driver/Inc/Ssd1306Driver.hpp"

// HAL（共通インタフェース）
#include "Hal/Inc/ICommInterface.hpp"

// STM32 HAL実装（BGFX_USE_STM32 定義時のみ有効）
#ifdef BGFX_USE_STM32
#include "Hal/Inc/Stm32I2c.hpp"
#include "Hal/Inc/Stm32I2cDma.hpp"
#endif

#endif /* BINARYGFX_BINARYGFX_HPP_ */
