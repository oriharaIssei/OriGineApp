#include "BackFrontAdaptSystem.h"

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

BackFrontAdaptSystem::BackFrontAdaptSystem()
    : ISystem(SystemType::Movement) {}

BackFrontAdaptSystem::~BackFrontAdaptSystem() {}

void BackFrontAdaptSystem::Initialize() {
    time_ = 0.0f;
}

void BackFrontAdaptSystem::Finalize() {}

void BackFrontAdaptSystem::UpdateEntity(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* blockCombiEntity             = ecsManager->getUniqueEntity("BlockCombination");

    if (!blockCombiEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    BlockStatus* blockStatus                 = getComponent<BlockStatus>(_entity);
    ModelMeshRenderer* modelRenderer         = getComponent<ModelMeshRenderer>(_entity);
    BlockCombinationStatus* blockCombiStatus = getComponent<BlockCombinationStatus>(blockCombiEntity);
    float deltaTime                          = Engine::getInstance()->getDeltaTime();

    if (!blockStatus || !modelRenderer || !blockCombiStatus) {
        return;
    }

    switch (blockStatus->GetFrontPlaneStatus()->GetPlaneStep()) {
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case BlockFrontPlaneStatus::PlaneStep::NONE:
        if (blockStatus->GetRowNum() < blockCombiStatus->GetConbinationMax()) {
            blockStatus->GetFrontPlaneStatus()->Reset();
            break;
        }

        blockStatus->GetFrontPlaneStatus()->SetPlaneStep(BlockFrontPlaneStatus::PlaneStep::CLOSE);
        break;
        ///---------------------------------------------------
        /// 適応
        ///---------------------------------------------------

   case BlockFrontPlaneStatus::PlaneStep::CLOSE:
        blockStatus->GetFrontPlaneStatus()->CloseEasing(deltaTime);
        break;
        ///---------------------------------------------------
        /// 終わり
        ///---------------------------------------------------
   case BlockFrontPlaneStatus::PlaneStep::END:
        /*color_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);*/
        break;
    default:
        break;
    }
    /* modelRenderer->getMaterialBuff(0)->color_ = color_;*/
}

void BackFrontAdaptSystem::ComboReset() {
}
