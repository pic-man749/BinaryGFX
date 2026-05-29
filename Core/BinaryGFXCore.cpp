/*
 * BinaryGFXCore.cpp
 *
 *      Author: picman
 */

#include <algorithm>
#include "BinaryGFXCore.hpp"

namespace BinaryGFX {

  BinaryGFX::BinaryGFX(std::unique_ptr<Driver::IDisplayDriver> driver) :
      m_driver(std::move(driver)),
      m_frameBuffers{
          FrameBuffer(m_driver->getWidth(), m_driver->getHeight()),
          FrameBuffer(m_driver->getWidth(), m_driver->getHeight())
      },
      m_activeBuffer(1u),
      m_nextId(1u) {
  }

  bool BinaryGFX::init() {
    return m_driver->init();
  }

  void BinaryGFX::removeObject(ObjectId id) {
    const auto it = std::find_if(m_objects.begin(), m_objects.end(), [id](const ObjectEntry &entry) {
      return entry.id == id;
    });
    if(it != m_objects.end()) {
      m_objects.erase(it);
    }
  }

  void BinaryGFX::removeAll() {
    m_objects.clear();
  }

  ErrorCode BinaryGFX::update() {
    // 前回と別のバッファへレンダリングする
    const uint8_t renderIdx = m_activeBuffer ^ 1u;
    m_frameBuffers[renderIdx].clear();
    renderAll(m_frameBuffers[renderIdx]);

    // レンダリング済みバッファを切り替える
    // DMA使用時: sendBuffer()内で前フレームDMA完了を待ってから転送開始する
    m_activeBuffer = renderIdx;
    return m_driver->sendBuffer(m_frameBuffers[m_activeBuffer].getData(), m_frameBuffers[m_activeBuffer].getDataSize());
  }

  void BinaryGFX::renderAll(FrameBuffer &fb) {
    std::sort(m_objects.begin(), m_objects.end(), [](const ObjectEntry &a, const ObjectEntry &b) {
      if(a.object->getZ() != b.object->getZ()) {
        return a.object->getZ() < b.object->getZ();
      }
      return a.id < b.id;
    });
    for(const auto &entry : m_objects) {
      entry.object->render(fb);
    }
  }

} // namespace BinaryGFX
