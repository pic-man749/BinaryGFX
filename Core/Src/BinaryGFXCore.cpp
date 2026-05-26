/*
 * BinaryGFXCore.cpp
 *
 *      Author: picman
 */

#include <algorithm>
#include "../Inc/BinaryGFXCore.hpp"

namespace BinaryGFX {

  BinaryGFX::BinaryGFX(std::unique_ptr<Driver::IDisplayDriver> driver) :
      m_driver(std::move(driver)), m_frameBuffer(m_driver->getWidth(), m_driver->getHeight()), m_nextId(1u) {
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
    m_frameBuffer.clear();
    renderAll();
    return m_driver->sendBuffer(m_frameBuffer.getData(), m_frameBuffer.getDataSize());
  }

  void BinaryGFX::renderAll() {
    std::sort(m_objects.begin(), m_objects.end(), [](const ObjectEntry &a, const ObjectEntry &b) {
      if(a.object->getZ() != b.object->getZ()) {
        return a.object->getZ() < b.object->getZ();
      }
      return a.id < b.id;
    });
    for(const auto &entry : m_objects) {
      entry.object->render(m_frameBuffer);
    }
  }

} // namespace BinaryGFX
