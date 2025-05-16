#include "DeleteBlockForAdvantageSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockStatus.h"
#include "component/EffectByBlock/EffectByBlockSpawner.h"
#include "component/Score/ScoreStatus.h"
#include "component/Timer/TimerStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

DeleteBlockForAdvantageSystem::DeleteBlockForAdvantageSystem() : ISystem(SystemType::StateTransition) {}

void DeleteBlockForAdvantageSystem::Initialize() {
}

void DeleteBlockForAdvantageSystem::Finalize() {
    /*entities_.clear();*/
}

DeleteBlockForAdvantageSystem::~DeleteBlockForAdvantageSystem() {}

void DeleteBlockForAdvantageSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();
    blockStatus_    = getComponent<BlockStatus>(_entity);

    if (!blockStatus_) {
        return;
    }

    // アドバンテージブロックによって破壊
    if (blockStatus_->GetIsBreakForAdvantageEffect()) {

        // タイマーゼロで消滅
        blockStatus_->BreakTimerDecrement(deltaTime);
        if (blockStatus_->GetBreakOffsetTime() > 0) {
            return;
        }

        /*   BlockReaction(_entity, blockStatus_->GetBlockType());*/
        DestroyEntity(_entity);
    }
}

void DeleteBlockForAdvantageSystem::BlockReaction(GameEntity* _entity, BlockType blocktype) {
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scoreEntity                  = ecsManager->getUniqueEntity("Score");
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");
    GameEntity* effectByBlockSpawner         = ecsManager->getUniqueEntity("effectByBlockSpawner");

    /// sound
    GameEntity* blockManager = ecsManager->getUniqueEntity("BlockManager");
    Audio* breakNormal       = getComponent<Audio>(blockManager, 5); // 落ちる音
    Audio* breakSkull        = getComponent<Audio>(blockManager, 6); // 落ちる音
    Audio* breakAdvance      = getComponent<Audio>(blockManager, 7); // 落ちる音

    if (!scoreEntity || !timerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    ScoreStatus* scoreStatus  = getComponent<ScoreStatus>(scoreEntity);
    TimerStatus* timerStatus  = getComponent<TimerStatus>(timerEntity);
    SpawnerStatus_            = getComponent<EffectByBlockSpawner>(effectByBlockSpawner);
    Transform* blockTransform = getComponent<Transform>(_entity);

    if (!scoreStatus || !timerStatus || !SpawnerStatus_ /*|| !combiEntity*/) { // Componentが存在しない場合の早期リターン
        return;
    }

    switch (blocktype) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:

        effectType_ = EffectType::TIME;
        breakNormal->Play();
        break;

        ///---------------------------------------------
        /// Skull
        ///---------------------------------------------
    case BlockType::SKULL:
        effectType_ = EffectType::TIME;

        breakSkull->Play();

        break;

        ///---------------------------------------------
        /// Advance
        ///---------------------------------------------
    case BlockType::ADVANTAGE:
        effectType_ = EffectType::MIMUSTIME;
        breakAdvance->Play();
        break;

    default:
        break;
    }

    SpawnerStatus_->EffectUISpawn(Vec3f(blockTransform->worldMat[3]), tempValue_, effectType_);
}

void DeleteBlockForAdvantageSystem::ApearResultScoreUI() {
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* combiEntity                  = ecsManager->getUniqueEntity("BlockCombination");
    BlockCombinationStatus* combiStatus      = getComponent<BlockCombinationStatus>(combiEntity);

    // effectType 判定
    if (blockStatus_->GetResultTime() >= 0) {
        effectType_ = EffectType::TIME;
    } else {
        effectType_ = EffectType::MIMUSTIME;
    }

    /*breakAdvance->Play();*/

    SpawnerStatus_->EffectUISpawn(Vec3f(combiStatus->GetEndPosition()), blockStatus_->GetResultTime(), effectType_);
    SpawnerStatus_->EffectUISpawn(Vec3f(combiStatus->GetEndPosition()), blockStatus_->GetResultScore(), EffectType::SCORE);
}
