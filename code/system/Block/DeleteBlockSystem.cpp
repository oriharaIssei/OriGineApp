#include "DeleteBlockSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include"component/Block/BlockStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

DeleteBlockSystem::DeleteBlockSystem() : ISystem(SystemType::StateTransition) {}

void DeleteBlockSystem::Initialize() {
}

void DeleteBlockSystem::Finalize() {
    /*entities_.clear();*/
}

DeleteBlockSystem::~DeleteBlockSystem() {}


void DeleteBlockSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }


    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* blockManagerEntity           = ecsManager->getUniqueEntity("BlockManager");

    if (!blockManagerEntity) {
        return;
    }

    Transform* transform       = getComponent<Transform>(_entity);
    BlockManager* blockManager = getComponent<BlockManager>(blockManagerEntity);

   

    if (!transform || !blockManager) {
        return;
    }

    if (transform->translate[X] <= blockManager->GetDeadPosition()) {
        DestroyEntity(_entity);
    }
}
