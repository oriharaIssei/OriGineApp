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
}

void ComboUIScrollSystem::Finalize() {}

void ComboUIScrollSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* ComboEntity                  = ecsManager->getUniqueEntity("Combo");

    if (!ComboEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    ComboStatus* comboStatus     = getComponent<ComboStatus>(ComboEntity);
    ComboUIStatus* comboUIStatus = getComponent<ComboUIStatus>(_entity);
    SpriteRenderer* spriteRenderer = getComponent<SpriteRenderer>(_entity);

    if (!comboStatus || !comboUIStatus || !spriteRenderer) { // Componentが存在しない場合の早期リターン
        return;
    }

    int32_t currentCombo           = comboStatus->GetCurrentComboNum();
   

     float uvoffset = static_cast<float>(comboUIStatus->GetValueForDigit(currentCombo)) * 0.1f; // 0.1fはUVのオフセット
    spriteRenderer->setUVTranslate(Vec2f(uvoffset, 0.0f));
}
