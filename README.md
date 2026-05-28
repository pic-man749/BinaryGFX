# BinaryGFX

モダンなC++言語で書ける、単色ディスプレイ用のグラフィックスライブラリです。

## 特徴

- SSD1306等を用いた単色OLEDをターゲットにしたグラフィックスライブラリ
- スマートポインタ・テンプレートを使ったモダンC++でコーディング（要C++17以降）

## 検証済みの環境

- ディスプレイドライバIC：
    - SSD1306（128×64、I2C 接続）
- MCU：
    - STM32 F401
- 開発環境：
    - STM32CubeIDE 2.0.0

---

## ディレクトリ構成

```
BinaryGFX/
├── BinaryGFX.hpp               # アンブレラヘッダ
├── Common/
│   └── Inc/                    # 共通型定義（ErrorCode, ObjectId, PixelState）
├── Core/
│   ├── Inc/
│   │   ├── BinaryGFXCore.hpp   # コアクラス
│   │   ├── FrameBuffer.hpp     # フレームバッファ
│   │   └── Objects/            # グラフィックオブジェクト
│   └── Src/
│       └── Objects/
├── Driver/
│   ├── Inc/
│   │   ├── IDisplayDriver.hpp  # ドライバインタフェース
│   │   └── Ssd1306Driver.hpp   # SSD1306 ドライバ実装
│   └── Src/
└── Hal/
    ├── Inc/
    │   ├── ICommInterface.hpp  # 通信インタフェース
    │   └── Stm32I2c.hpp        # STM32 I2C ラッパー実装
    │   └── Stm32I2cDma.hpp     # STM32 I2C (DMA) ラッパー実装
    └── Src/
```

---

## アーキテクチャ

```
┌──────────────────────────────────────────────────────┐     ┌───────────────────────────────┐
│  Core (BinaryGFX::)                                  │     │  Common                       │
│  BinaryGFX, FrameBuffer, IGraphicsObject, Objects    │ ref │  ErrorCode, ObjectId,         │
│                         ↓ uses                       │  →  |  InvalidObjectId, PixelState  |
│              IDisplayDriver (interface)              │     |                               |
└──────────────────────────────────────────────────────┘     |                               |
                          ↑ implements                       |                               |
┌──────────────────────────────────────────────────────┐     |                               |
│  Driver (BinaryGFX::Driver::)                        │     |                               |
│  Ssd1306Driver                                       │  →  |                               |
│                         ↓ uses                       │     |                               |
│              ICommInterface (interface)              │     |                               |
└──────────────────────────────────────────────────────┘     |                               |
                          ↑ implements                       |                               |
┌──────────────────────────────────────────────────────┐     |                               |
│  HAL (BinaryGFX::Hal::)                              │     |                               |
│  Stm32I2c                                            │  →  |                               |
└──────────────────────────────────────────────────────┘     └───────────────────────────────┘
```

各層の境界はインタフェース（純粋仮想クラス）で定義されるため、`IDisplayDriver` や `ICommInterface` を自前で実装することで任意のディスプレイ・通信手段に対応できます。

---

## セットアップ

### 1. STM32CubeMX の設定

HAL クラス（`Stm32I2c` / `Stm32I2cDma`）は STM32CubeMX が生成する `i2c.h` をインクルードします。
STM32CubeMX のコード生成設定で以下の項目を有効にしてください。

**Project Manager → Code Generator**

| 設定項目 | 値 |
|----------|----|
| Generate peripheral initialization as a pair of '.c/.h' files per peripheral | チェックを入れる |

これにより、I2C ペリフェラルの初期化コードが `i2c.c` / `i2c.h` として分離生成されます。

#### DMA を使用する場合（`Stm32I2cDma`）

`Stm32I2cDma` を使用する際は、I2C の DMA 転送設定も STM32CubeMX で行う必要があります。

1. **Connectivity → I2C** の設定画面を開く
2. **DMA Settings** タブで `I2C_TX`（送信方向）の DMA リクエストを追加する

> DMA 転送完了コールバック（`HAL_I2C_MasterTxCpltCallback`）が必要な場合は、アプリケーション側で実装してください。

---

### 2. プリプロセッサマクロの設定

`BinaryGFX.hpp` は `BGFX_USE_STM32` が定義されている場合のみ STM32 向け HAL クラス（`Stm32I2c` / `Stm32I2cDma`）をインクルードします。

STM32CubeIDE でマクロを定義するには、プロジェクトプロパティ → **C/C++ Build → Settings → MCU GCC Compiler → Preprocessor** に以下を追加します。

```
BGFX_USE_STM32
```

または、`BinaryGFX.hpp` のインクルード前にソースコード上で定義することもできます。

```cpp
#define BGFX_USE_STM32
#include "BinaryGFX.hpp"
```

---

### 4. インクルードパスの追加

STM32CubeIDE のプロジェクトプロパティ → **C/C++ Build → Settings → MCU GCC Compiler → Include paths** に以下を追加します。

```
../UserLib/BinaryGFX
```

### 5. ソースファイルの追加

プロジェクトに以下の `.cpp` ファイルが含まれていることを確認してください。STM32CubeIDE ではプロジェクトツリーに追加されていれば自動的にビルド対象になります。

```
UserLib/BinaryGFX/Core/Src/BinaryGFX.cpp
UserLib/BinaryGFX/Core/Src/FrameBuffer.cpp
UserLib/BinaryGFX/Core/Src/Objects/PointObject.cpp
UserLib/BinaryGFX/Core/Src/Objects/LineObject.cpp
UserLib/BinaryGFX/Core/Src/Objects/RectangleObject.cpp
UserLib/BinaryGFX/Core/Src/Objects/CircleObject.cpp
UserLib/BinaryGFX/Core/Src/Objects/TriangleObject.cpp
UserLib/BinaryGFX/Driver/Src/Ssd1306Driver.cpp
UserLib/BinaryGFX/Hal/Src/Stm32I2c.cpp
```

---

## 使い方

### 基本的な使用例

```cpp
#include <memory>
#include "i2c.h"
#include "BinaryGFX.hpp"

// 各層を生成して BinaryGFX へ依存注入する
auto comm   = std::make_unique<BinaryGFX::Hal::Stm32I2c>(&hi2c1, 100 /*ms*/);
auto driver = std::make_unique<BinaryGFX::Driver::Ssd1306Driver>(
                  std::move(comm), 0x3C /*7bit addr*/, 128, 64);
auto gfx = std::make_unique<BinaryGFX::BinaryGFX>(std::move(driver));

// 初期化
if (!gfx->init()) {
    // 初期化失敗の処理
}

// オブジェクトを追加する
gfx->addObject(std::make_unique<BinaryGFX::RectangleObject>(0, 0, 128, 64));

// idを保持しておくことで、追加したオブジェクトをあとから操作できます
auto circleId = gfx->addObject(
    std::make_unique<BinaryGFX::CircleObject>(64, 32, 20, BinaryGFX::PixelState::On, true));

// 描画・転送
gfx->update();
```

### アニメーション（オブジェクトの更新）

`addObject()` が返す `ObjectId` を使ってオブジェクトを取得し、プロパティを変更してから再描画できます。

```cpp
auto* circle = gfx->getObjectById<BinaryGFX::CircleObject>(circleId);
if (circle) {
    circle->setCenter(80, 32); // 位置を変更
}
gfx->update(); // 変更を反映して転送
```

---

## グラフィックオブジェクト一覧

| クラス | 説明 | 主なセッター |
|--------|------|-----------|
| `PointObject` | 点 | `setPosition()`, `setPixelState()` |
| `LineObject` | 直線 | `setStart()`, `setEnd()`, `setPixelState()` |
| `RectangleObject` | 矩形 | `setPosition()`, `setSize()`, `setPixelState()`, `setFilled()` |
| `CircleObject` | 円 | `setCenter()`, `setRadius()`, `setPixelState()`, `setFilled()` |
| `TriangleObject` | 三角形 | `setVertices()`, `setPixelState()`, `setFilled()` |

すべてのオブジェクトに共通の `setZ(int16_t z)` で描画順を制御できます。Z 値が小さいオブジェクトほど先に描画されます。

---

## API リファレンス

### BinaryGFX（コアクラス）

```cpp
namespace BinaryGFX {

// コンストラクタ: ドライバの所有権を受け取りフレームバッファを初期化する
explicit BinaryGFX(std::unique_ptr<Driver::IDisplayDriver> driver);

// ディスプレイを初期化する
bool init();

// オブジェクトを追加する（所有権を移譲）。戻り値の ObjectId で後から参照できる
template<typename T> ObjectId addObject(std::unique_ptr<T> obj);

// ObjectId でオブジェクトを取得する（removeObject() 後にポインタは無効になる）
template<typename T> T* getObjectById(ObjectId id);

// 指定 ID のオブジェクトを削除する
void removeObject(ObjectId id);

// 全オブジェクトを削除する
void removeAll();

// フレームバッファをクリアして全オブジェクトを再描画し、ディスプレイへ転送する
ErrorCode update();

} // namespace BinaryGFX
```

### 共通型

```cpp
namespace BinaryGFX {

using ObjectId = uint32_t;
static constexpr ObjectId InvalidObjectId = 0u;

enum class PixelState : uint8_t {
    Off = 0,  // 消灯
    On  = 1,  // 点灯
};

enum class ErrorCode : uint8_t {
    Ok,
    Timeout,
    Nack,
    BusError,
    NotInitialized,
    InvalidArgument,
};

} // namespace BinaryGFX
```

---

## 拡張方法

### 別のディスプレイに対応する

`BinaryGFX::Driver::IDisplayDriver` を実装することで任意のディスプレイに対応できます。

```cpp
class MyDisplay : public BinaryGFX::Driver::IDisplayDriver {
public:
    bool init() override { /* 初期化処理 */ return true; }
    BinaryGFX::ErrorCode sendBuffer(const uint8_t* data, size_t size) override { /* 転送処理 */ }
    void setDisplayOn(bool on) override { /* ON/OFF 切替 */ }
    uint16_t getWidth()  const override { return 128; }
    uint16_t getHeight() const override { return 64; }
};
```

### 別の通信手段に対応する

`BinaryGFX::Hal::ICommInterface` を実装することで SPI・UART など任意の通信方式を使用できます。

```cpp
class MySpi : public BinaryGFX::Hal::ICommInterface {
public:
    BinaryGFX::ErrorCode write(uint8_t addr, const uint8_t* data, size_t size) override { /* 送信処理 */ }
    BinaryGFX::ErrorCode read (uint8_t addr, uint8_t* data, size_t size) override { /* 受信処理 */ }
};
```

---

## ライセンス

MIT License
