#include "DeleteBlockSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include"component/Block/BlockStatus.h"
#include"component/Score/ScoreStatus.h"
#include"component/Timer/TimerStatus.h"
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
    blockStatus_               = getComponent<BlockStatus>(_entity);
   

    if (!transform || !blockManager || !blockStatus_) {
        return;
    }

    if (transform->translate[X] <= blockManager->GetDeadPosition()) {
        BlockReaction(blockStatus_->GetBlockType());
        DestroyEntity(_entity);
    }
}

void DeleteBlockSystem::BlockReaction(BlockType blockType) {
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scoreEntity                  = ecsManager->getUniqueEntity("Score");
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");

    if (!scoreEntity || !timerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    ScoreStatus* scoreStatus = getComponent<ScoreStatus>(scoreEntity);
    TimerStatus* timerStatus = getComponent<TimerStatus>(timerEntity);

    if (!scoreStatus || !timerStatus) { // Componentが存在しない場合の早期リターン
        return;
    }

    float timerDecrementValue = timerStatus->GetMinusTimeNormal();
    float scoreValue          = blockStatus_->GetBaseScoreValue();

    switch (blockType) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:
        timerStatus->TimerDecrement(timerDecrementValue);
        break;

        ///---------------------------------------------
        /// Skull
        ///---------------------------------------------
    case BlockType::SKULL:
        scoreStatus->PlusScoreIncrement(scoreValue);
        scoreStatus->SetScoreChangeTime(0.0f);
        break;

        ///---------------------------------------------
        /// Advance
        ///---------------------------------------------
    case BlockType::ADVANTAGE:
        scoreStatus->PlusScoreIncrement(scoreValue);
        scoreStatus->SetScoreChangeTime(0.0f);
        break;

    default:
        break;
    }
 }
