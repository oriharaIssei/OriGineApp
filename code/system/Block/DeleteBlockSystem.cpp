#include "DeleteBlockSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include"component/Block/BlockStatus.h"
#include"component/EffectByBlock/EffectByBlockSpawner.h"
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
        BlockReaction(_entity,blockStatus_->GetBlockType());
        DestroyEntity(_entity);
    }
}

void DeleteBlockSystem::BlockReaction(GameEntity* _entity, BlockType blocktype) {
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scoreEntity                  = ecsManager->getUniqueEntity("Score");
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");
    GameEntity* effectByBlockSpawner         = ecsManager->getUniqueEntity("effectByBlockSpawner");

    if (!scoreEntity || !timerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    ScoreStatus* scoreStatus = getComponent<ScoreStatus>(scoreEntity);
    TimerStatus* timerStatus = getComponent<TimerStatus>(timerEntity);
    EffectByBlockSpawner* SpawnerStatus = getComponent<EffectByBlockSpawner>(effectByBlockSpawner);

    if (!scoreStatus || !timerStatus || !SpawnerStatus) { // Componentが存在しない場合の早期リターン
        return;
    }

    float timerDecrementValue = timerStatus->GetMinusTimeNormal();
    float scoreValue          = blockStatus_->GetBaseScoreValue();

    switch (blocktype) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:
        timerStatus->TimerDecrement(timerDecrementValue);
        effectType_ = EffectType::MIMUSTIME;
        tempValue_  = timerDecrementValue;
        break;

        ///---------------------------------------------
        /// Skull
        ///---------------------------------------------
    case BlockType::SKULL:
        scoreStatus->PlusScoreIncrement(scoreValue);
        scoreStatus->SetScoreChangeTime(0.0f);
        effectType_ = EffectType::SCORE;
        tempValue_  = scoreValue;
        break;

        ///---------------------------------------------
        /// Advance
        ///---------------------------------------------
    case BlockType::ADVANTAGE:
        scoreStatus->PlusScoreIncrement(scoreValue);
        scoreStatus->SetScoreChangeTime(0.0f);
        effectType_ = EffectType::SCORE;
        tempValue_  = scoreValue;
        break;

    default:
        break;
    }

       SpawnerStatus->EffectUISpawn(_entity, tempValue_, effectType_);
 }
