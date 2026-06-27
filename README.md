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
│   ├── Error.hpp               # 共通型定義（ErrorCode）
│   └── PixelState.hpp          # 共通型定義（PixelState）
├── Core/
│   ├── BinaryGFXCore.hpp / .cpp  # コアクラス
│   ├── ObjectId.hpp              # オブジェクトID定義（ObjectId, TypedObjectId）
│   ├── ObjectFactory.hpp         # オブジェクト生成ヘルパー（create*関数）
│   ├── FrameBuffer.hpp / .cpp    # フレームバッファ
│   ├── Font/
│   │   ├── FontData.hpp          # フォントデータ構造体
│   │   └── BgfxFont_Ascii.hpp    # デフォルトのフォント
│   ├── Binary/
│   │   └── BinaryData.hpp        # バイナリ画像データ構造体
│   └── Objects/                  # グラフィックオブジェクト
│       ├── IGraphicsObject.hpp
│       ├── PointObject.hpp / .cpp
│       ├── LineObject.hpp / .cpp
│       ├── RectangleObject.hpp / .cpp
│       ├── CircleObject.hpp / .cpp
│       ├── TriangleObject.hpp / .cpp
│       ├── TextObject.hpp / .cpp
│       ├── StringObject.hpp / .cpp
│       └── BinaryObject.hpp / .cpp
├── Driver/
│   ├── IDisplayDriver.hpp        # ドライバインタフェース
│   └── Ssd1306Driver.hpp / .cpp  # SSD1306 ドライバ実装
└── Hal/
    ├── ICommInterface.hpp        # 通信インタフェース
    ├── Stm32I2c.hpp / .cpp       # STM32 I2C ラッパー実装
    ├── Stm32I2cDma.hpp / .cpp    # STM32 I2C (DMA) ラッパー実装
    └── Helper/
        └── I2cHelper.hpp / .cpp  # I2C ステータス変換ヘルパー
```

---

## アーキテクチャ

```
┌──────────────────────────────────────────────────────┐     ┌───────────────────────────────┐
│  Core (BinaryGFX::)                                  │     │  Common                       │
│  BinaryGFX, ObjectId, TypedObjectId, FrameBuffer,    │ ref │  ErrorCode, PixelState        │
│  IGraphicsObject, Objects                            │  →  |                               |
│                         ↓ uses                       │     |                               |
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
BinaryGFX
```

### 5. ソースファイルの追加

プロジェクトに以下の `.cpp` ファイルが含まれていることを確認してください。STM32CubeIDE ではプロジェクトツリーに追加されていれば自動的にビルド対象になります。

```
BinaryGFX/Core/BinaryGFXCore.cpp
BinaryGFX/Core/FrameBuffer.cpp
BinaryGFX/Core/Objects/PointObject.cpp
BinaryGFX/Core/Objects/LineObject.cpp
BinaryGFX/Core/Objects/RectangleObject.cpp
BinaryGFX/Core/Objects/CircleObject.cpp
BinaryGFX/Core/Objects/TriangleObject.cpp
BinaryGFX/Core/Objects/TextObject.cpp
BinaryGFX/Core/Objects/StringObject.cpp
BinaryGFX/Core/Objects/BinaryObject.cpp
BinaryGFX/Driver/Ssd1306Driver.cpp
BinaryGFX/Hal/Stm32I2c.cpp
BinaryGFX/Hal/Helper/I2cHelper.cpp
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
BinaryGFX::createRectangle(*gfx, 0, 0, 128, 64);

// idを保持しておくことで、追加したオブジェクトをあとから操作できます
auto circleId = BinaryGFX::createCircle(*gfx, 64, 32, 20, BinaryGFX::PixelState::On, true);

// 描画・転送
gfx->update();
```

> `Core/ObjectFactory.hpp` が提供する `create*` ヘルパー関数を利用することで簡易にオブジェクトを追加できます。 `gfx->addObject(std::make_unique<BinaryGFX::RectangleObject>(...))` のように直接オブジェクトの記述を追加することも可能です。

### テキストの表示

テキストを表示するには、`FontData` 構造体でフォントデータを定義した上で `TextObject` を追加します。

```cpp
// フォントデータ定義（縦ページ形式・LSB が上端・列優先）
// 1グリフ = glyphWidth × ceil(glyphHeight/8) バイト
// データ配置: [col0_page0, col0_page1, ..., col1_page0, ...]
constexpr uint8_t myFontData[] = {
    // ' ' (0x20)
    0x00, 0x00, 0x00, 0x00, 0x00,
    // '!' (0x21)
    0x00, 0x00, 0x5F, 0x00, 0x00,
    // ... 以降 0x7E まで続く
};

// typedef struct のため struct キーワード不要
constexpr BinaryGFX::FontData myFont = {
    5,           // glyphWidth  (ピクセル)
    8,           // glyphHeight (ピクセル)
    0x20,        // firstChar
    95,          // charCount   (0x20〜0x7E)
    myFontData
};

// TextObject を追加する（ワードラップはデフォルトで有効）
BinaryGFX::createText(*gfx, 0, 0, "Hello!", &myFont);

// 文字間・行間スペースやワードラップはセッターで設定できる
auto id = BinaryGFX::createText(*gfx, 0, 16, "Line1\nLine2", &myFont);
auto* text = gfx->getObjectById(id); // idの型からTextObjectが自動的に決まる
if(text) {
    text->setCharSpacing(2);   // 文字間 2px
    text->setLineSpacing(2);   // 行間 2px
    text->setWordWrap(false);  // ワードラップ無効（デフォルト: 有効）
}

gfx->update();
```

### バイナリ画像の表示

ロゴやアイコンなどの任意のビットマップ画像を表示するには、`BinaryData` 構造体でデータを定義した上で `BinaryObject` を追加します。

```cpp
// データ形式: 縦ページ形式・LSB が上端・列優先（FontData と同じ形式）
// データサイズ = width × ceil(height / 8) バイト
// データ配置: [col0_page0, col0_page1, ..., col1_page0, ...]
constexpr uint8_t logoData[] = {
    // 8×8 の枠線サンプル（width=8, height=8 → 8列 × 1ページ = 8バイト）
    0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF,
};

constexpr BinaryGFX::BinaryData logo = {
    8,        // width  (ピクセル)
    8,        // height (ピクセル)
    logoData
};

// BinaryObject を追加する
BinaryGFX::createBinary(*gfx, 10, 10, &logo);

gfx->update();
```

---

### アニメーション（オブジェクトの更新）

`addObject()` が返す `TypedObjectId<T>` を使ってオブジェクトを取得し、プロパティを変更してから再描画できます。

```cpp
auto* circle = gfx->getObjectById(circleId); // idの型からCircleObjectが自動的に決まる
if (circle) {
    circle->setCenter(80, 32); // 位置を変更
}
gfx->update(); // 変更を反映して転送
```

---

## グラフィックオブジェクト一覧

| クラス | 説明 | ヘルパー関数 | 主なセッター |
|--------|------|-----------|-----------|
| `PointObject` | 点 | `createPoint()` | `setPosition()`, `setPixelState()` |
| `LineObject` | 直線 | `createLine()` | `setStart()`, `setEnd()`, `setPixelState()` |
| `RectangleObject` | 矩形 | `createRectangle()` | `setPosition()`, `setSize()`, `setPixelState()`, `setFilled()` |
| `CircleObject` | 円 | `createCircle()` | `setCenter()`, `setRadius()`, `setPixelState()`, `setFilled()` |
| `TriangleObject` | 三角形 | `createTriangle()` | `setVertices()`, `setPixelState()`, `setFilled()` |
| `TextObject` | テキスト（`const char*`、所有権なし） | `createText()` | `setPosition()`, `setText()`, `setFont()`, `setPixelState()`, `setCharSpacing()`, `setLineSpacing()`, `setWordWrap()` |
| `StringObject` | テキスト（`std::string`として文字列を保持） | `createString()` | `setPosition()`, `setText()`, `getText()`, `setFont()`, `setPixelState()`, `setCharSpacing()`, `setLineSpacing()`, `setWordWrap()` |
| `BinaryObject` | バイナリ画像（ロゴ・アイコン等） | `createBinary()` | `setPosition()`, `setBinaryData()`, `setPixelState()` |

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

// オブジェクトを追加する（所有権を移譲）。戻り値の TypedObjectId<T> で後から参照できる
template<typename T> TypedObjectId<T> addObject(std::unique_ptr<T> obj);

// TypedObjectId<T> でオブジェクトを取得する（removeObject() 後にポインタは無効になる）
// idの型からTが自動的に決まるため、呼び出し時にテンプレート引数を明示する必要はない
template<typename T> T* getObjectById(TypedObjectId<T> id);

// 指定 ID のオブジェクトを削除する
void removeObject(ObjectId id);

// 全オブジェクトを削除する
void removeAll();

// フレームバッファをクリアして全オブジェクトを再描画し、ディスプレイへ転送する
ErrorCode update();

} // namespace BinaryGFX
```

### オブジェクト生成ヘルパー（ObjectFactory）

`Core/ObjectFactory.hpp` は、各グラフィックオブジェクトの `std::make_unique` + `addObject()` 呼び出しを簡略化するヘルパー関数を提供します。

```cpp
namespace BinaryGFX {

TypedObjectId<PointObject>     createPoint(BinaryGFX &gfx, int16_t x, int16_t y, PixelState pixelState = PixelState::On, int16_t z = 0);
TypedObjectId<LineObject>      createLine(BinaryGFX &gfx, int16_t x0, int16_t y0, int16_t x1, int16_t y1, PixelState pixelState = PixelState::On, int16_t z = 0);
TypedObjectId<RectangleObject> createRectangle(BinaryGFX &gfx, int16_t x, int16_t y, int16_t w, int16_t h, PixelState pixelState = PixelState::On, bool filled = false, int16_t z = 0);
TypedObjectId<CircleObject>    createCircle(BinaryGFX &gfx, int16_t cx, int16_t cy, int16_t r, PixelState pixelState = PixelState::On, bool filled = false, int16_t z = 0);
TypedObjectId<TriangleObject>  createTriangle(BinaryGFX &gfx, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, PixelState pixelState = PixelState::On, bool filled = false, int16_t z = 0);
TypedObjectId<TextObject>      createText(BinaryGFX &gfx, int16_t x, int16_t y, const char *text, const FontData *font, PixelState pixelState = PixelState::On, int16_t z = 0);
TypedObjectId<StringObject>    createString(BinaryGFX &gfx, int16_t x, int16_t y, std::string string, const FontData *font, PixelState pixelState = PixelState::On, int16_t z = 0);
TypedObjectId<BinaryObject>    createBinary(BinaryGFX &gfx, int16_t x, int16_t y, const BinaryData *data, PixelState pixelState = PixelState::On, int16_t z = 0);

} // namespace BinaryGFX
```

各関数は対応する `XxxObject` のコンストラクタへ引数をそのまま転送し、`gfx.addObject(std::make_unique<XxxObject>(...))` を実行した結果を返す薄いラッパーです。戻り値は `addObject()` と同じ `TypedObjectId<T>` です。

### 共通型

```cpp
namespace BinaryGFX {

using ObjectId = uint32_t;
static constexpr ObjectId InvalidObjectId = 0u;

// 型情報を保持するオブジェクトID。addObject()のみが有効な値を発行できる
template<typename T> class TypedObjectId {
public:
    constexpr TypedObjectId(); // 無効なIDとして構築する
    constexpr bool isValid() const;
    constexpr operator ObjectId() const; // 型消去（removeObject()等に使用）
};

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
