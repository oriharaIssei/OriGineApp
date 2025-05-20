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
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();

    GameEntity* effectByBlockSpawner = ecsManager->getUniqueEntity("effectByBlockSpawner");

    SpawnerStatus_ = getComponent<EffectByBlockSpawner>(effectByBlockSpawner);
    blockStatus_   = getComponent<BlockStatus>(_entity);
    blockTransform_ = getComponent<Transform>(_entity);

    float deltaTime = Engine::getInstance()->getDeltaTime();

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

        if (blockStatus_->GetIsRightEdge()) {
            ApearResultScoreUI();
        }

        DestroyEntity(_entity);
    }
}

void DeleteBlockForAdvantageSystem::BlockReaction(GameEntity* _entity, BlockType blocktype) {
    _entity;
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scoreEntity                  = ecsManager->getUniqueEntity("Score");
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");

    /// sound
    GameEntity* blockManager = ecsManager->getUniqueEntity("BlockManager");
    Audio* breakNormal       = getComponent<Audio>(blockManager, 5); // 落ちる音
    Audio* breakSkull        = getComponent<Audio>(blockManager, 6); // 落ちる音
    Audio* breakAdvance      = getComponent<Audio>(blockManager, 7); // 落ちる音
    Emitter* normalStartEmitter   = getComponent<Emitter>(blockManager, 0);//通常エミッタその1
    Emitter* normalIconEmitter  = getComponent<Emitter>(blockManager, 1);
  /*  Transform* baseTransform      = getComponent<Transform>(_entity, 1);*/

    if (!scoreEntity || !timerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    ScoreStatus* scoreStatus  = getComponent<ScoreStatus>(scoreEntity);
    TimerStatus* timerStatus  = getComponent<TimerStatus>(timerEntity);
  

    if (!scoreStatus || !timerStatus || !SpawnerStatus_ /*|| !combiEntity*/) { // Componentが存在しない場合の早期リターン
        return;
    }

    switch (blocktype) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:

        effectType_ = EffectType::TIME;
       /* normalStartEmitter->setParent(baseTransform);
        normalIconEmitter->setParent(baseTransform);*/

        //parent じゃなく emitterの初期位置を指定したい(setPositionみたいに)

        normalStartEmitter->PlayStart();
        normalIconEmitter->PlayStart();
        breakNormal->Play();
        break;

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

    SpawnerStatus_->EffectUISpawn(Vec3f(blockTransform_->worldMat[3]), tempValue_, effectType_);
}

void DeleteBlockForAdvantageSystem::ApearResultScoreUI() {
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* combiEntity = ecsManager->getUniqueEntity("BlockCombination");

    // effectType 判定
    if (blockStatus_->GetResultTime() >= 0) {
        timeEffectType_ = EffectType::TIME;
    } else {
        timeEffectType_ = EffectType::MIMUSTIME;
    }

    // effectType 判定
    if (blockStatus_->GetResultScore() >= 0) {
        scoreEffectType_ = EffectType::SCORE;
    } else {
        scoreEffectType_ = EffectType::MIMUSSCORE;
    }

     BlockCombinationStatus* breakAdvance = getComponent<BlockCombinationStatus>(combiEntity); // 落ちる音

    SpawnerStatus_->EffectUISpawn(Vec3f(blockTransform_->worldMat[3]) + breakAdvance->GetTimerUIPosOffset(), std::fabsf(blockStatus_->GetResultTime()), timeEffectType_);
     SpawnerStatus_->EffectUISpawn(Vec3f(blockTransform_->worldMat[3]) + breakAdvance->GetScoreUIPosOffset(), std::fabsf(blockStatus_->GetResultScore()), scoreEffectType_);
}
