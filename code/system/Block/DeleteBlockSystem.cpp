#include "DeleteBlockSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Block/BlockStatus.h"
#include "component/EffectByBlock/EffectByBlockSpawner.h"
#include "component/Score/ScoreStatus.h"
#include "component/Timer/TimerStatus.h"
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
        BlockReaction(_entity, blockStatus_->GetBlockType());
        transform->scale = Vec3f(0.0f, 0.0f, 0.0f);
        DestroyEntity(_entity);
    }
}

void DeleteBlockSystem::BlockReaction(GameEntity* _entity, BlockType blocktype) {
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scoreEntity                  = ecsManager->getUniqueEntity("Score");
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");
    GameEntity* effectByBlockSpawner         = ecsManager->getUniqueEntity("effectByBlockSpawner");

    /// sound
    GameEntity* blockManager = ecsManager->getUniqueEntity("BlockManager");
    Audio* breakNormal       = getComponent<Audio>(blockManager, 2); // 落ちる音
    Audio* breakSkull        = getComponent<Audio>(blockManager, 3); // 落ちる音
    Audio* breakAdvance      = getComponent<Audio>(blockManager, 4); // 落ちる音

    if (!scoreEntity || !timerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    ScoreStatus* scoreStatus            = getComponent<ScoreStatus>(scoreEntity);
    TimerStatus* timerStatus            = getComponent<TimerStatus>(timerEntity);
    EffectByBlockSpawner* SpawnerStatus = getComponent<EffectByBlockSpawner>(effectByBlockSpawner);
    Transform* blockTransform           = getComponent<Transform>(_entity);

    if (!scoreStatus || !timerStatus || !SpawnerStatus || !blockTransform) { // Componentが存在しない場合の早期リターン
        return;
    }

    float timerDecrementValue = timerStatus->GetMinusTimeNormal();
    float scoreValue          = blockStatus_->GetBaseScoreValue();

    switch (blocktype) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:
        timerStatus->MinusTimer(timerDecrementValue);
        effectType_ = EffectType::MIMUSTIME;
        tempValue_  = timerDecrementValue;
        /* if (!breakNormal->isPlaying()) {*/
        breakNormal->Play();
        /*}*/
        break;

        ///---------------------------------------------
        /// Skull
        ///---------------------------------------------
    case BlockType::SKULL:
        scoreStatus->PlusScoreIncrement(scoreValue);
        scoreStatus->ScoreUPChange();
        effectType_ = EffectType::SCORE;
        tempValue_  = scoreValue;
        /* if (!breakNormal->isPlaying()) {*/
        breakSkull->Play();
        /*}*/
        break;

        ///---------------------------------------------
        /// Advance
        ///---------------------------------------------
    case BlockType::ADVANTAGE:
        scoreStatus->PlusScoreIncrement(scoreValue);
        scoreStatus->ScoreUPChange();
        effectType_ = EffectType::SCORE;
        tempValue_  = scoreValue;
        /* if (!breakNormal->isPlaying()) {*/
        breakAdvance->Play();
        /*}*/
        break;

    default:
        break;
    }

    SpawnerStatus->EffectUISpawn(Vec3f(blockTransform->worldMat[3]), tempValue_, effectType_);
}
