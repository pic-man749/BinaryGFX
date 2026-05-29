/*
 * Ssd1306Driver.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_DRIVER_INC_SSD1306DRIVER_HPP_
#define BINARYGFX_DRIVER_INC_SSD1306DRIVER_HPP_

#include <cstdint>
#include <memory>
#include "IDisplayDriver.hpp"
#include "../Hal/ICommInterface.hpp"

namespace BinaryGFX::Driver {

  /**
   * @brief SSD1306 OLEDディスプレイドライバ
   *
   * I2C経由でSSD1306を制御するディスプレイドライバ。
   * フレームバッファは縦ページ形式（SSD1306 GDDRAMと同一レイアウト）を前提とする。
   */
  class Ssd1306Driver: public IDisplayDriver {
    public:

      /**
       * @brief コンストラクタ
       *
       * @param comm       通信インタフェース（所有権を移譲）
       * @param deviceAddr デバイスアドレス（7ビット値）
       * @param width      ディスプレイ幅（ピクセル）
       * @param height     ディスプレイ高さ（ピクセル）
       */
      Ssd1306Driver(std::unique_ptr<Hal::ICommInterface> comm, uint8_t deviceAddr, uint16_t width, uint16_t height);

      /**
       * @brief ディスプレイを初期化する
       *
       * SSD1306標準初期化コマンドシーケンスを送信し、表示をONにする。
       *
       * @return true 初期化成功
       * @return false 初期化失敗
       */
      bool init() override;

      /**
       * @brief フレームバッファデータをGDDRAMへ転送する
       *
       * カラム・ページアドレスを設定後、フレームバッファ全体を送信する。
       *
       * @param data フレームバッファデータ（縦ページ形式）
       * @param size データサイズ（バイト）
       * @return ErrorCode 転送結果
       */
      ErrorCode sendBuffer(const uint8_t *data, size_t size) override;

      /**
       * @brief ディスプレイの表示ON/OFFを切り替える
       *
       * @param on true:表示ON（0xAF）、false:表示OFF（0xAE）
       */
      void setDisplayOn(bool on) override;

      /**
       * @brief ディスプレイの幅を返す
       *
       * @return ディスプレイ幅（ピクセル）
       */
      uint16_t getWidth() const override;

      /**
       * @brief ディスプレイの高さを返す
       *
       * @return ディスプレイ高さ（ピクセル）
       */
      uint16_t getHeight() const override;

    private:

      /**
       * @brief 1バイトのコマンドを送信する
       *
       * コントロールバイト 0x00 を先頭に付けて送信する。
       *
       * @param cmd 送信するコマンドバイト
       * @return ErrorCode 送信結果
       */
      ErrorCode sendCommand(uint8_t cmd);

      /**
       * @brief データを送信する
       *
       * コントロールバイト 0x40 を先頭に付けて送信する。
       *
       * @param data 送信データ
       * @param size データサイズ（バイト）
       * @return ErrorCode 送信結果
       */
      ErrorCode sendData(const uint8_t *data, size_t size);

      std::unique_ptr<Hal::ICommInterface> m_comm; /**< 通信インタフェース */
      uint8_t m_deviceAddr; /**< デバイスアドレス（7ビット値） */
      uint16_t m_width; /**< ディスプレイ幅（ピクセル） */
      uint16_t m_height; /**< ディスプレイ高さ（ピクセル） */
  };

} // namespace BinaryGFX::Driver

#endif /* BINARYGFX_DRIVER_INC_SSD1306DRIVER_HPP_ */
