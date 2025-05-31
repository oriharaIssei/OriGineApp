#include "DeleteBlockForAdvantageSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockStatus.h"
#include "component/DeleteEntityStatus/DeleteEntityStatus.h"
#include "component/EffectByBlock/EffectByBlockSpawner.h"
#include "engine/EngineInclude.h"
// sys
#include "system/DeleteEntitySystem/DeleteEntitySystem.h"
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

    SpawnerStatus_  = getComponent<EffectByBlockSpawner>(effectByBlockSpawner);
    blockStatus_    = getComponent<BlockStatus>(_entity);
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
        // 右端にUI出す
        if (blockStatus_->GetIsRightEdge()) {
            ApearResultScoreUI();
        }
        // 破壊パーティクル
        BlockBreakParticleShot(_entity, blockStatus_->GetBlockType());
        blockTransform_->scale = Vec3f(0.0f, 0.0f, 0.0f);
        DestroyEntity(_entity);
    }
}

void DeleteBlockForAdvantageSystem::BlockBreakParticleShot(GameEntity* _entity, BlockType blocktype) {
    _entity;
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();

    /// sound
    GameEntity* blockManager = ecsManager->getUniqueEntity("BlockManager");
    Audio* breakNormal       = getComponent<Audio>(blockManager, 8); // 落ちる音
    Audio* breakSkull        = getComponent<Audio>(blockManager, 9); // 落ちる音
    Audio* breakAdvance      = getComponent<Audio>(blockManager, 10); // 落ちる音

    // Emitter* normalStartEmitter = getComponent<Emitter>(_entity, 0); // 通常エミッタその1
    // Emitter* normalIconEmitter  = getComponent<Emitter>(_entity, 1);

    if (!SpawnerStatus_) { // Componentが存在しない場合の早期リターン
        return;
    }

    SpawnBlockEffect(blocktype);

    switch (blocktype) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:

        breakNormal->Play();
        break;

        /// Skull
        ///---------------------------------------------
    case BlockType::SKULL:

        breakSkull->Play();

        break;

        ///---------------------------------------------
        /// Advance
        ///---------------------------------------------
    case BlockType::ADVANTAGE:

        breakAdvance->Play();
        break;

    default:
        break;
    }
}

void DeleteBlockForAdvantageSystem::SpawnBlockEffect(BlockType type) {
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* breakBlockEffectManager      = ecsManager->getUniqueEntity("AdAndSkullBreakEffect");

    GameEntity* blockbreakEffect = CreateEntity<Emitter, Emitter, Emitter, DeleteEntityStatus>("blockbreakEffect", Emitter(), Emitter(), Emitter(), DeleteEntityStatus());

    // advantage
    for (int32_t i = 0; i < breakBlockEffects_.size(); ++i) {
        breakBlockEffects_[i] = getComponent<Emitter>(breakBlockEffectManager, i);
    }

    for (int32_t i = 0; i < breakBlockEffects_.size(); ++i) {
        breakEffect_[i] = getComponent<Emitter>(blockbreakEffect, i);
    }

    /*  Emitter* breakEffect0 = getComponent<Emitter>(blockbreakEffect, 0);
      Emitter* breakEffect1 = getComponent<Emitter>(blockbreakEffect, 1);
      Emitter* breakEffect2 = getComponent<Emitter>(blockbreakEffect, 2);*/

    DeleteEntityStatus* deleteEntityStatus = getComponent<DeleteEntityStatus>(blockbreakEffect);
    deleteEntityStatus->SetDeleteTime(5.0f);

    switch (type) {
    case BlockType::NORMAL:

        break;
    case BlockType::ADVANTAGE:
        *breakEffect_[0] = *breakBlockEffects_[0];
        *breakEffect_[1] = *breakBlockEffects_[1];
        *breakEffect_[2] = *breakBlockEffects_[2];
        break;
    case BlockType::SKULL:
        break;
    case BlockType::COUNT:
        break;
    default:
        break;
    }

    //// Particle発射
    Vec3f basePos = Vec3f(blockTransform_->worldMat[3]) + Vec3f(0.0f, 0.0f, -5.0f);

    ECSManager* ecs = ECSManager::getInstance();
    //------------------ Input
    // None

    //------------------ StateTransition
    ecs->getSystem<DeleteEntitySystem>()->addEntity(blockbreakEffect);
    //------------------ Movement
    ecs->getSystem<EmitterWorkSystem>()->addEntity(blockbreakEffect);
    //------------------ Collision

    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<ParticleRenderSystem>()->addEntity(blockbreakEffect);


    // set origin 
    for (int32_t i = 0; i < breakBlockEffects_.size(); ++i) {
        breakEffect_[i]->setOriginePos(basePos);
        breakEffect_[i]->PlayStart();
    }

    /*breakEffect0->setOriginePos(basePos);
    breakEffect1->setOriginePos(basePos);
    breakEffect2->setOriginePos(basePos);

    breakEffect0->PlayStart();
    breakEffect1->PlayStart();
    breakEffect2->PlayStart();*/
}

void DeleteBlockForAdvantageSystem::ApearResultScoreUI() {
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* combiEntity                  = ecsManager->getUniqueEntity("BlockCombination");

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
