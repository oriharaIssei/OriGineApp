#include "BreakBlockSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include"component/Block/BlockStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

BreakBlockSystem::BreakBlockSystem() : ISystem(SystemType::StateTransition) {}

void BreakBlockSystem::Initialize() {
}

void BreakBlockSystem::Finalize() {
    /*entities_.clear();*/
}

BreakBlockSystem::~BreakBlockSystem() {}


void BreakBlockSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }


   /*  EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* blockManagerEntity           = ecsManager->getUniqueEntity("BlockManager");

    if (!blockManagerEntity) {
        return;
    }*/

    BlockStatus* blockStatus = getComponent<BlockStatus>(_entity);
    /*BlockManager* blockManager = getComponent<BlockManager>(blockManagerEntity);*/

   

    if (!blockStatus) {
        return;
    }

    if (blockStatus->GetIsBreak()) {
        DestroyEntity(_entity);
    }
}
