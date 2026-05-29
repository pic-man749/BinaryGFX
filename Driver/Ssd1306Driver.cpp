/*
 * Ssd1306Driver.cpp
 *
 *      Author: picman
 */

#include <vector>
#include "../Inc/Ssd1306Driver.hpp"

namespace {

  // @formatter:off
  // ---- I2C コントロールバイト ----
  constexpr uint8_t CTRL_CMD  = 0x00u;  /**< コマンドモード: 後続バイトをすべてコマンドとして送信 */
  constexpr uint8_t CTRL_DATA = 0x40u;  /**< データモード: 後続バイトをすべてデータとして送信 */

  // ---- SSD1306 コマンドコード ----
  constexpr uint8_t CMD_DISPLAY_OFF         = 0xAEu;  /**< ディスプレイ OFF */
  constexpr uint8_t CMD_DISPLAY_ON          = 0xAFu;  /**< ディスプレイ ON */
  constexpr uint8_t CMD_SET_CLOCK_DIV       = 0xD5u;  /**< ディスプレイクロック分周比 / 発振周波数設定 */
  constexpr uint8_t CMD_SET_MULTIPLEX       = 0xA8u;  /**< マルチプレクス比設定 */
  constexpr uint8_t CMD_SET_DISPLAY_OFFSET  = 0xD3u;  /**< ディスプレイオフセット設定 */
  constexpr uint8_t CMD_SET_START_LINE_0    = 0x40u;  /**< ディスプレイスタートライン = 0 */
  constexpr uint8_t CMD_CHARGE_PUMP         = 0x8Du;  /**< チャージポンプ設定 */
  constexpr uint8_t CMD_SET_MEMORY_MODE     = 0x20u;  /**< メモリアドレッシングモード設定 */
  constexpr uint8_t CMD_SEG_REMAP           = 0xA1u;  /**< セグメントリマップ（列127をSEG0へ） */
  constexpr uint8_t CMD_COM_SCAN_DEC        = 0xC8u;  /**< COM出力スキャン方向（COM[N-1]→COM0）*/
  constexpr uint8_t CMD_SET_COM_PINS        = 0xDAu;  /**< COMピンハードウェア構成設定 */
  constexpr uint8_t CMD_SET_CONTRAST        = 0x81u;  /**< コントラスト設定 */
  constexpr uint8_t CMD_SET_PRECHARGE       = 0xD9u;  /**< プリチャージ期間設定 */
  constexpr uint8_t CMD_SET_VCOMH           = 0xDBu;  /**< VCOMH デセレクトレベル設定 */
  constexpr uint8_t CMD_DISPLAY_ALL_ON_RESUME = 0xA4u;  /**< 全画面ON無効（GDDRAMに従う） */
  constexpr uint8_t CMD_NORMAL_DISPLAY      = 0xA6u;  /**< 通常表示（反転なし） */
  constexpr uint8_t CMD_DEACTIVATE_SCROLL   = 0x2Eu;  /**< スクロール無効 */
  constexpr uint8_t CMD_SET_COLUMN_ADDR     = 0x21u;  /**< カラムアドレス設定 */
  constexpr uint8_t CMD_SET_PAGE_ADDR       = 0x22u;  /**< ページアドレス設定 */

  // ---- 初期化パラメータ値 ----
  constexpr uint8_t  VAL_CLOCK_DIV              = 0x80u;  /**< 標準クロック分周比 */
  constexpr uint8_t  VAL_CHARGE_PUMP_ENABLE     = 0x14u;  /**< チャージポンプ有効 */
  constexpr uint8_t  VAL_MEMORY_MODE_HORIZONTAL = 0x00u;  /**< 水平アドレッシングモード */
  constexpr uint8_t  VAL_COM_PINS_ALT           = 0x12u;  /**< 代替COMピン構成（高さ64px用） */
  constexpr uint8_t  VAL_COM_PINS_SEQ           = 0x02u;  /**< 順次COMピン構成（高さ32px用） */
  constexpr uint8_t  VAL_CONTRAST_HIGH          = 0xCFu;  /**< 高コントラスト設定（高さ64px用） */
  constexpr uint8_t  VAL_CONTRAST_LOW           = 0x8Fu;  /**< 低コントラスト設定（高さ32px用） */
  constexpr uint8_t  VAL_PRECHARGE              = 0xF1u;  /**< プリチャージ期間値 */
  constexpr uint8_t  VAL_VCOMH_DESELECT         = 0x40u;  /**< VCOMH デセレクトレベル値 */
  constexpr uint16_t HEIGHT_THRESHOLD           = 64u;    /**< 64px以上を高さ大として扱う閾値 */
  // @formatter:on

} // namespace

namespace BinaryGFX::Driver {

  Ssd1306Driver::Ssd1306Driver(std::unique_ptr<Hal::ICommInterface> comm, uint8_t deviceAddr, uint16_t width, uint16_t height) :
      m_comm(std::move(comm)), m_deviceAddr(deviceAddr), m_width(width), m_height(height) {
  }

  bool Ssd1306Driver::init() {
    // ディスプレイ高さに依存するパラメータを決定する
    const uint8_t comPins  = (m_height >= HEIGHT_THRESHOLD) ? VAL_COM_PINS_ALT   : VAL_COM_PINS_SEQ;
    const uint8_t contrast = (m_height >= HEIGHT_THRESHOLD) ? VAL_CONTRAST_HIGH  : VAL_CONTRAST_LOW;

    // @formatter:off
    // SSD1306 標準初期化コマンドシーケンス（1回のI2C転送で送信）
    const uint8_t cmds[] = {
        CTRL_CMD,                                                     // コントロールバイト: 以降すべてコマンドモード
        CMD_DISPLAY_OFF,                                              // ディスプレイ OFF
        CMD_SET_MULTIPLEX,       static_cast<uint8_t>(m_height - 1u), // マルチプレクス比（高さ-1）
        CMD_SET_DISPLAY_OFFSET,  0x00u,                               // ディスプレイオフセット = 0
        CMD_SET_START_LINE_0,                                         // ディスプレイスタートライン = 0
        CMD_SEG_REMAP,                                                // セグメントリマップ（列127をSEG0へ）
        CMD_COM_SCAN_DEC,                                             // COM出力スキャン方向（COM[N-1]→COM0）
        CMD_SET_COM_PINS,        comPins,                             // COMピンハードウェア構成
        CMD_SET_CONTRAST,        contrast,                            // コントラスト設定
        CMD_DISPLAY_ALL_ON_RESUME,                                    // 全画面ON無効（GDDRAMに従う）
        CMD_NORMAL_DISPLAY,                                           // 通常表示（反転なし）
        CMD_SET_CLOCK_DIV,       VAL_CLOCK_DIV,                       // ディスプレイクロック分周比 / 発振周波数
        CMD_CHARGE_PUMP,         VAL_CHARGE_PUMP_ENABLE,              // チャージポンプ有効化
        CMD_SET_MEMORY_MODE,     VAL_MEMORY_MODE_HORIZONTAL,          // メモリアドレッシングモード: 水平
        CMD_DEACTIVATE_SCROLL,                                        // スクロール無効
        CMD_DISPLAY_ON,                                               // ディスプレイ ON
    };
    // @formatter:on
    return m_comm->write(m_deviceAddr, cmds, sizeof(cmds)) == ErrorCode::Ok;
  }

  ErrorCode Ssd1306Driver::sendBuffer(const uint8_t *data, size_t size) {
    // カラムアドレス設定（0 〜 width-1）
    const uint8_t colCmd[] = {
        CTRL_CMD, CMD_SET_COLUMN_ADDR,
        0x00u,
        static_cast<uint8_t>(m_width - 1u)
    };
    ErrorCode ec = m_comm->write(m_deviceAddr, colCmd, sizeof(colCmd));
    if(ec != ErrorCode::Ok) {
      return ec;
    }

    // ページアドレス設定（0 〜 height/8-1）
    const uint8_t pageCmd[] = {
        CTRL_CMD, CMD_SET_PAGE_ADDR,
        0x00u,
        static_cast<uint8_t>(m_height / 8u - 1u)
    };
    ec = m_comm->write(m_deviceAddr, pageCmd, sizeof(pageCmd));
    if(ec != ErrorCode::Ok) {
      return ec;
    }

    // フレームバッファ全体を転送
    return sendData(data, size);
  }

  void Ssd1306Driver::setDisplayOn(bool on) {
    sendCommand(on ? CMD_DISPLAY_ON : CMD_DISPLAY_OFF);
  }

  uint16_t Ssd1306Driver::getWidth() const {
    return m_width;
  }

  uint16_t Ssd1306Driver::getHeight() const {
    return m_height;
  }

  ErrorCode Ssd1306Driver::sendCommand(uint8_t cmd) {
    // CTRL_CMD: 後続バイトをコマンドとして送信
    const uint8_t buf[2] = {CTRL_CMD, cmd};
    return m_comm->write(m_deviceAddr, buf, sizeof(buf));
  }

  ErrorCode Ssd1306Driver::sendData(const uint8_t *data, size_t size) {
    // CTRL_DATA を先頭に付加してデータとして送信
    std::vector<uint8_t> packet;
    packet.reserve(size + 1u);
    packet.push_back(CTRL_DATA);
    packet.insert(packet.end(), data, data + size);
    return m_comm->write(m_deviceAddr, packet.data(), packet.size());
  }

} // namespace BinaryGFX::Driver
