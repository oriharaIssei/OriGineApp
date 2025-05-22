#include "BlockApearSystem.h"

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
#include "component/Block/BlockStatus.h"

#include "engine/EngineInclude.h"
#include <Vector4.h>

BlockApearSystem::BlockApearSystem()
    : ISystem(SystemType::Movement) {}

BlockApearSystem::~BlockApearSystem() {}

void BlockApearSystem::Initialize() {
    time_ = 0.0f;
}

void BlockApearSystem::Finalize() {}

void BlockApearSystem::UpdateEntity(GameEntity* _entity) {

   /* EntityComponentSystemManager* ecsManager = ECSManager::getInstance();*/
   /* GameEntity* blockCombiEntity             = ecsManager->getUniqueEntity("BlockCombination");*/

    //if (!blockCombiEntity) { // Entityが存在しない場合の早期リターン
    //    return;
    //}

    BlockStatus* blockStatus                 = getComponent<BlockStatus>(_entity);
    Transform* transform = getComponent<Transform>(_entity);
  /*  BlockCombinationStatus* blockCombiStatus = getComponent<BlockCombinationStatus>(blockCombiEntity);*/
   /* Vec4f color_;*/

    float deltaTime = Engine::getInstance()->getDeltaTime();

    if (!blockStatus || !transform) {
        return;
    }

    switch (blockStatus->GetApearAnimationstep()) {
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case BlockStatus::ApearAnimationStep::NONE:
      
        blockStatus->TimerReset();
     
        break;
        ///---------------------------------------------------
        /// 適応
        ///---------------------------------------------------

    case BlockStatus::ApearAnimationStep::APEAR:
        blockStatus->ZApearEasing(deltaTime);
        break;
        ///---------------------------------------------------
        /// 適応
        ///---------------------------------------------------

    case BlockStatus::ApearAnimationStep::REVERSE:
        blockStatus->ZBackEasing(deltaTime);
        break;
        ///---------------------------------------------------
        /// 終わり
        ///---------------------------------------------------
    case BlockStatus::ApearAnimationStep::END:

        blockStatus->TimerReset();
        blockStatus->SetApearAnimationstep(BlockStatus::ApearAnimationStep::NONE);
        break;
    default:
        break;
    }

    transform->translate[Z] = blockStatus->GetZPosition();

}

void BlockApearSystem::ComboReset() {
}
