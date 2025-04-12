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

    if (!ComboEntity) {
        return;
    }

    ComboStatus* comboStatus = getComponent<ComboStatus>(ComboEntity);
}
