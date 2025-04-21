#include "BreakBlockSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Block/BlockStatus.h"
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

    BlockStatus* blockStatus = getComponent<BlockStatus>(_entity);

    if (!blockStatus) {
        return;
    }

    if (blockStatus->GetIsBreak()) {
        BlockReaction(blockStatus->GetBlockType());
        DestroyEntity(_entity);
    }
}

void BreakBlockSystem::BlockReaction(BlockType blocktype) {
    switch (blocktype) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:

        break;

        ///---------------------------------------------
        /// Skull
        ///---------------------------------------------
    case BlockType::SKULL:

        break;

        ///---------------------------------------------
        /// Advance
        ///---------------------------------------------
    case BlockType::ADVANTAGE:

        break;

    default:
        break;
    }
}
