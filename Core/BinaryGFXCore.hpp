/*
 * BinaryGFXCore.hpp
 *
 *      Author: picman
 */

#ifndef USERLIB_BINARYGFX_CORE_INC_BINARYGFXCORE_HPP_
#define USERLIB_BINARYGFX_CORE_INC_BINARYGFXCORE_HPP_

#include <cstdint>
#include <vector>
#include <memory>
#include "FrameBuffer.hpp"
#include "Objects/IGraphicsObject.hpp"
#include "../Common/Error.hpp"
#include "../Driver/IDisplayDriver.hpp"

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
       * デュアルバッファ方式で動作する。
       * DMA非使用時: レンダリング → 転送 → 返却（ブロッキング）
       * DMA使用時  : 前フレームDMA完了待ち → レンダリング → 転送開始 → 即返却
       * (前フレームのDMA転送中にレンダリングが並行実行される)
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
       * @brief 全オブジェクトを指定フレームバッファへレンダリングする
       *
       * Z値昇順（同値はObjectId昇順）でソートして各オブジェクトのrender()を呼び出す。
       *
       * @param fb レンダリング先フレームバッファ
       */
      void renderAll(FrameBuffer &fb);

      std::unique_ptr<Driver::IDisplayDriver> m_driver;  /**< ディスプレイドライバ */
      FrameBuffer m_frameBuffers[2];                     /**< デュアルフレームバッファ */
      uint8_t m_activeBuffer;                            /**< 直前のsendBuffer()で使用したバッファインデックス（0 or 1） */
      std::vector<ObjectEntry> m_objects;                /**< オブジェクト管理リスト */
      ObjectId m_nextId;                                 /**< 次に採番するObjectId */
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

#endif /* USERLIB_BINARYGFX_CORE_INC_BINARYGFXCORE_HPP_ */
