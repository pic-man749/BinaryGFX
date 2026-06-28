/*
 * AnimationObject.cpp
 *
 *      Author: picman
 */

#include "AnimationObject.hpp"

namespace BinaryGFX {

  AnimationObject::AnimationObject(int16_t x, int16_t y, const AnimationFrames *af,
                                   bool loop, PlaybackState state,
                                   PixelState pixelState,
                                   int16_t z) :
      BinaryObject(x, y, (af->frameCount > 0u) ? af->frames[0] : nullptr, pixelState, z),
      m_af(af),
      m_currentFrame(0u), m_lastUpdateTick(0u), m_loop(loop),
      // Playingで生成された場合、最初のupdate()では基準Tickの記録のみを行う
      m_pendingReset(state == PlaybackState::Playing),
      m_state(state) {
  }

  void AnimationObject::update(uint32_t tickMs) {
    if(m_state != PlaybackState::Playing) {
      return;
    }

    // 再生開始直後は基準Tickを記録するのみで、フレーム切替は行わない
    if(m_pendingReset) {
      m_lastUpdateTick = tickMs;
      m_pendingReset = false;
      return;
    }

    const uint32_t elapsed = tickMs - m_lastUpdateTick;
    if(elapsed < m_af->intervalMs) {
      return;
    }
    m_lastUpdateTick = tickMs;

    const uint16_t next = static_cast<uint16_t>(m_currentFrame + 1u);
    if(next >= m_af->frameCount) {
      if(!m_loop) {
        // 1回再生の終了：最終フレームを保持したまま停止する
        m_state = PlaybackState::Stopped;
        return;
      }
      m_currentFrame = 0u;
    } else {
      m_currentFrame = next;
    }
    setBinaryData(m_af->frames[m_currentFrame]);
  }

  void AnimationObject::playLoop() {
    startPlayback(true);
  }

  void AnimationObject::playOnce() {
    startPlayback(false);
  }

  void AnimationObject::stop() {
    m_state = PlaybackState::Stopped;
  }

  void AnimationObject::resume() {
    if(m_af->frameCount == 0u) {
      return;
    }
    m_state = PlaybackState::Playing;
    m_pendingReset = true;
  }

  AnimationObject::PlaybackState AnimationObject::getPlaybackState() const {
    return m_state;
  }

  void AnimationObject::startPlayback(bool loop) {
    m_loop = loop;
    m_currentFrame = 0u;
    if(m_af->frameCount > 0u) {
      setBinaryData(m_af->frames[0]);
    }
    m_state = PlaybackState::Playing;
    m_pendingReset = true;
  }

} // namespace BinaryGFX
