#include "MoveSpeedChangeSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include"component/Block/BlockManager.h"
#include"component/Timer/TimerStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

MoveSpeedChangeSystem::MoveSpeedChangeSystem() : ISystem(SystemType::Movement) {}

void MoveSpeedChangeSystem::Initialize() {
}

void MoveSpeedChangeSystem::Finalize() {
    /*entities_.clear();*/
}

MoveSpeedChangeSystem::~MoveSpeedChangeSystem() {}


void MoveSpeedChangeSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }


    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* timerManagerEntity           = ecsManager->getUniqueEntity("Timer");

    if (!timerManagerEntity) {
        return;
    }

    BlockManager* blockManager  = getComponent<BlockManager>(_entity);
    TimerStatus* timerStatus = getComponent<TimerStatus>(timerManagerEntity);

   

    if (!blockManager || !timerStatus) {
        return;
    }

    blockManager->SpeedChangeForTime(timerStatus->GetCurrentTimer());
   
}
