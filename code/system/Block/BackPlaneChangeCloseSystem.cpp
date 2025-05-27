#include "BackPlaneChangeCloseSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
// ECS
#define ENGINE_ECS
// include

// component
#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockFrontPlaneStatus.h"
#include "component/Block/BlockStatus.h"

#include "engine/EngineInclude.h"
#include <Vector4.h>

BackPlaneChangeCloseSystem::BackPlaneChangeCloseSystem()
    : ISystem(SystemType::StateTransition) {}

BackPlaneChangeCloseSystem::~BackPlaneChangeCloseSystem() {}

void BackPlaneChangeCloseSystem::Initialize() {
    time_ = 0.0f;
}

void BackPlaneChangeCloseSystem::Finalize() {}

void BackPlaneChangeCloseSystem::UpdateEntity(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* blockCombiEntity             = ecsManager->getUniqueEntity("BlockCombination");

    if (!blockCombiEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    BlockStatus* blockStatus                 = getComponent<BlockStatus>(_entity);
    BlockCombinationStatus* blockCombiStatus = getComponent<BlockCombinationStatus>(blockCombiEntity);
  /*  float deltaTime                          = Engine::getInstance()->getDeltaTime();*/

    if (!blockStatus ||  !blockCombiStatus) {
        return;
    }

    if (blockStatus->GetRowNum() >= blockCombiStatus->GetConbinationMax()) {
        blockStatus->SetIsCloseFrontBackPlane(true);
    }

     
}

void BackPlaneChangeCloseSystem::ComboReset() {
}
