#include "FrontPlaneDeleteSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include"component/Block/BlockFrontPlaneStatus.h"
#include"component/EffectByBlock/EffectByBlockSpawner.h"
#include"component/Score/ScoreStatus.h"
#include"component/Timer/TimerStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

FrontPlaneDeleteSystem::FrontPlaneDeleteSystem() : ISystem(SystemType::StateTransition) {}

void FrontPlaneDeleteSystem::Initialize() {
}

void FrontPlaneDeleteSystem::Finalize() {
    /*entities_.clear();*/
}

FrontPlaneDeleteSystem::~FrontPlaneDeleteSystem() {}


void FrontPlaneDeleteSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    BlockFrontPlaneStatus* frontblock = getComponent<BlockFrontPlaneStatus>(_entity);
   

    if (!frontblock) {
        return;
    }

    if (frontblock->GetIsDeath()) {

        DestroyEntity(_entity);
    }
}
