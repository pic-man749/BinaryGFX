/*
 * AnimationObject.hpp
 *
 *      Author: picman
 */

#ifndef BINARYGFX_CORE_OBJECTS_ANIMATIONOBJECT_HPP_
#define BINARYGFX_CORE_OBJECTS_ANIMATIONOBJECT_HPP_

#include <cstdint>
#include "BinaryObject.hpp"
#include "../../Common/PixelState.hpp"
#include "../Binary/AnimationFrames.hpp"

namespace BinaryGFX {

  /**
   * @brief バイナリ画像アニメーションオブジェクト
   *
   * 複数のBinaryDataフレームを一定間隔で差し替えて表示する。
   * フレーム配列・各BinaryDataのデータポインタの所有権は持たない
   * （ライフタイムはユーザが管理する）。
   */
  class AnimationObject: public BinaryObject {
    public:

      /**
       * @brief 再生状態
       */
      enum class PlaybackState : uint8_t {
        Stopped, /**< Stopped 停止中（1回再生の終了後もこの状態になる） */
        Playing, /**< Playing 再生中 */
      };

      /**
       * @brief コンストラクタ
       *
       * 初期表示フレームは frames[0] となる。
       *
       * @param x          描画開始X座標（左上基準）
       * @param y          描画開始Y座標（左上基準）
       * @param af         AnimationFramesへのポインタ
       * @param loop       true:ループ再生、false:1回再生（デフォルト: true）
       * @param state      初期再生状態（デフォルト: Playing）
       * @param pixelState 描画色（デフォルト: 点灯）
       * @param z          Z値（デフォルト: 0）
       */
      AnimationObject(int16_t x, int16_t y, const AnimationFrames *af,
                      bool loop = true,
                      PlaybackState state = PlaybackState::Playing,
                      PixelState pixelState = PixelState::On,
                      int16_t z = 0);

      /**
       * @brief 時間経過に応じて表示フレームを更新する
       *
       * @param tickMs 現在のTick値（ミリ秒）
       */
      void update(uint32_t tickMs) override;

      /**
       * @brief 先頭フレームからループ再生を開始する
       */
      void playLoop();

      /**
       * @brief 先頭フレームから1回再生を開始する
       *
       * 最終フレームまで到達すると自動的にStopped状態へ遷移する。
       */
      void playOnce();

      /**
       * @brief 再生を停止する
       *
       * 現在の表示フレームを保持したまま再生を停止する。
       */
      void stop();

      /**
       * @brief 停止位置から再生を再開する
       *
       * playLoop()/playOnce()で設定した再生モード（ループ有無）を維持したまま再開する。
       */
      void resume();

      /**
       * @brief 現在の再生状態を取得する
       *
       * @return PlaybackState 現在の再生状態
       */
      PlaybackState getPlaybackState() const;

    private:

      /**
       * @brief 再生を開始する共通処理
       *
       * @param loop true:ループ再生、false:1回再生
       */
      void startPlayback(bool loop);

      const AnimationFrames *m_af; /**< AnimationFramesポインタ */
      uint16_t m_currentFrame; /**< 現在表示中のフレーム番号 */
      uint32_t m_lastUpdateTick; /**< 直前にフレームを切り替えたTick値 */
      bool m_loop; /**< true:ループ再生、false:1回再生 */
      bool m_pendingReset; /**< true時、次回update()でm_lastUpdateTickを再設定する */
      PlaybackState m_state; /**< 現在の再生状態 */
  };

} // namespace BinaryGFX

#endif /* BINARYGFX_CORE_OBJECTS_ANIMATIONOBJECT_HPP_ */
