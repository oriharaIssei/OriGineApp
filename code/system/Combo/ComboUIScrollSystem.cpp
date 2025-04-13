#include "ComboUIScrollSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Combo/ComboStatus.h"
#include "component/Combo/ComboUIStatus.h"

#include "engine/EngineInclude.h"
#include <cmath>
#include <cstdint>

ComboUIScrollSystem::ComboUIScrollSystem()
    : ISystem(SystemType::Movement) {}

ComboUIScrollSystem::~ComboUIScrollSystem() {}

void ComboUIScrollSystem::Initialize() {
    // TextureNameの初期化
    for (int32_t i = 0; i < 10; ++i) {
        textureName_[i] = "resource/Texture/Combo/ComboNumber" + std::to_string(i) + ".png";
    };
}

void ComboUIScrollSystem::Finalize() {}

void ComboUIScrollSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* ComboEntity                  = ecsManager->getEntity(96);//<-これが取得できない(今はidで対応)

    if (!ComboEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    ComboStatus* comboStatus         = getComponent<ComboStatus>(ComboEntity);
    ComboUIStatus* comboUIStatus     = getComponent<ComboUIStatus>(_entity);
    ModelMeshRenderer* modelRenderer = getComponent<ModelMeshRenderer>(_entity);

    if (!comboStatus || !comboUIStatus || !modelRenderer) { // Componentが存在しない場合の早期リターン
        return;
    }

    // 現在コンボの取得
    int32_t currentCombo = comboStatus->GetCurrentComboNum();

    if (comboUIStatus->GetCurrentTextureName() == textureName_[comboUIStatus->GetValueForDigit(currentCombo)]) {
        return; // すでに同じテクスチャが設定されている場合は何もしない
    }
    /// コンボの数字よってTextureを変える
    modelRenderer->setTexture(0, textureName_[comboUIStatus->GetValueForDigit(currentCombo)]);

    // 現在のテクスチャ名を更新
    comboUIStatus->SetcurrentTextureName(textureName_[comboUIStatus->GetValueForDigit(currentCombo)]);
}
