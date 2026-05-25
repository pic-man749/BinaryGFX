/*
 * BinaryGFX.hpp
 *
 *      Author: picman
 */

#ifndef USERLIB_BINARYGFX_CORE_INC_BINARYGFX_HPP_
#define USERLIB_BINARYGFX_CORE_INC_BINARYGFX_HPP_

#include <cstdint>
#include <vector>
#include <memory>
#include "FrameBuffer.hpp"
#include "Objects/IGraphicsObject.hpp"
#include "../../Common/Inc/Error.hpp"
#include "../../Driver/Inc/IDisplayDriver.hpp"

namespace BinaryGFX {

  /**
   * @brief BinaryGFXコアクラス
   *
   * グラフィックオブジェクトを管理し、フレームバッファへのレンダリングと
   * ディスプレイへの転送を担う。1インスタンスが1ディスプレイに対応する。
   */
  class BinaryGFX {
    public:

      /**
       * @brief コンストラクタ
       *
       * フレームバッファはdriverから取得した幅・高さで初期化される。
       * driverのgetWidth()/getHeight()はinit()前から有効である必要がある。
       *
       * @param driver ディスプレイドライバ（所有権を移譲）
       */
      explicit BinaryGFX(std::unique_ptr<Driver::IDisplayDriver> driver);

      /**
       * @brief ディスプレイを初期化する
       *
       * @return true 初期化成功、false 初期化失敗
       */
      bool init();

      /**
       * @brief グラフィックオブジェクトを追加する
       *
       * オブジェクトの所有権を移譲して管理リストへ追加する。
       *
       * @tparam T  IGraphicsObjectを継承した具体的なオブジェクト型
       * @param obj 追加するオブジェクト
       * @return ObjectId 採番されたオブジェクトID。nullptrが渡された場合はInvalidObjectId
       */
      template<typename T>
      ObjectId addObject(std::unique_ptr<T> obj);

      /**
       * @brief ObjectIdでグラフィックオブジェクトを取得する
       *
       * 返した生ポインタはremoveObject()呼び出し後に無効になる。
       * ポインタは保持せず毎回本メソッドで取得すること。
       *
       * @tparam T IGraphicsObjectを継承した具体的なオブジェクト型
       * @param id 取得するオブジェクトのID
       * @return T* オブジェクトへの生ポインタ。見つからない場合はnullptr
       */
      template<typename T>
      T* getObjectById(ObjectId id);

      /**
       * @brief グラフィックオブジェクトを削除する
       *
       * @param id 削除するオブジェクトのID
       */
      void removeObject(ObjectId id);

      /**
       * @brief 全グラフィックオブジェクトを削除する
       */
      void removeAll();

      /**
       * @brief フレームバッファを更新してディスプレイへ転送する
       *
       * フレームバッファをクリアした後、Z値昇順（同値はObjectId昇順）で
       * 各オブジェクトをレンダリングし、ドライバ経由でディスプレイへ転送する。
       *
       * @return ErrorCode 転送結果
       */
      ErrorCode update();

    private:

      /**
       * @brief オブジェクト管理エントリ
       */
      struct ObjectEntry {
          ObjectId id; /**< オブジェクトID */
          std::unique_ptr<IGraphicsObject> object; /**< グラフィックオブジェクト */
      };

      /**
       * @brief 全オブジェクトをフレームバッファへレンダリングする
       *
       * Z値昇順（同値はObjectId昇順）でソートして各オブジェクトのrender()を呼び出す。
       */
      void renderAll();

      std::unique_ptr<Driver::IDisplayDriver> m_driver; /**< ディスプレイドライバ */
      FrameBuffer m_frameBuffer; /**< フレームバッファ */
      std::vector<ObjectEntry> m_objects; /**< オブジェクト管理リスト */
      ObjectId m_nextId; /**< 次に採番するObjectId */
  };

  template<typename T>
  ObjectId BinaryGFX::addObject(std::unique_ptr<T> obj) {
    if(!obj) {
      return InvalidObjectId;
    }
    const ObjectId id = m_nextId++;
    m_objects.push_back(ObjectEntry { id, std::move(obj) });
    return id;
  }

  template<typename T>
  T* BinaryGFX::getObjectById(ObjectId id) {
    for(auto &entry : m_objects) {
      if(entry.id == id) {
        return static_cast<T*>(entry.object.get());
      }
    }
    return nullptr;
  }

} // namespace BinaryGFX

#endif /* USERLIB_BINARYGFX_CORE_INC_BINARYGFX_HPP_ */
