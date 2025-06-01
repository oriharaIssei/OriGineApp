#include "CombiArrayRemoveSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include"component/Block/BlockCombinationStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

CombiArrayRemoveSystem::CombiArrayRemoveSystem() : ISystem(SystemType::StateTransition) {}

void CombiArrayRemoveSystem::Initialize() {
}

void CombiArrayRemoveSystem::Finalize() {
    /*entities_.clear();*/
}

CombiArrayRemoveSystem::~CombiArrayRemoveSystem() {}

void CombiArrayRemoveSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }
    BlockCombinationStatus* blockCombi = getComponent<BlockCombinationStatus>(_entity);

    if (!blockCombi) {
        return;
    }

    // remove
    blockCombi->RemoveMarkedBlocks();
}

