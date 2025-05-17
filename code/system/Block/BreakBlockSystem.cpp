#include "BreakBlockSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
/// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#define ENGINE_COMPONENTS
// component
#include "component/Block/BlockStatus.h"
#include "component/EffectByBlock/EffectByBlockSpawner.h"
#include "component/Score/ScoreStatus.h"
#include "component/Scrap/ScrapSpawner.h"
#include "component/Scrap/ScrapStatus.h"
#include "component/Timer/TimerStatus.h"
/// system
#include "component/Block/BlockCombinationStatus.h"
#include "system/scrap/ScrapDeleteSystem.h"
#include "system/scrap/ScrapFallSystem.h"
#include "system/scrap/ScrapToPlayerCollisionSystem.h"
#include <cstdint>
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

    blockStatus_ = getComponent<BlockStatus>(_entity);

    if (!blockStatus_) {
        return;
    }

    if (blockStatus_->GetIsBreak()) {

        if (!blockStatus_->GetIsBreakForAdvantageEffect()) {
           /* ScrapSpawn(_entity);*/
            BlockReaction(_entity, blockStatus_->GetBlockType());
        }
        DestroyEntity(_entity);
    }
}

void BreakBlockSystem::BlockReaction(GameEntity* _entity, BlockType blocktype) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scoreEntity                  = ecsManager->getUniqueEntity("Score");
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");
    GameEntity* effectByBlockSpawner         = ecsManager->getUniqueEntity("effectByBlockSpawner");
    GameEntity* combiEntity                  = ecsManager->getUniqueEntity("BlockCombination");

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
    BlockCombinationStatus* combiStatus = getComponent<BlockCombinationStatus>(combiEntity);
    Transform* blockTransform           = getComponent<Transform>(_entity);

    if (!scoreStatus || !timerStatus || !SpawnerStatus || !combiStatus) { // Componentが存在しない場合の早期リターン
        return;
    }

    /*  float timerDecrementValue = timerStatus->GetMinusTime() * blockStatus_->GetPlusScoreRate();*/
    float plusTimerValue = timerStatus->GetPulusTime() * blockStatus_->GetPlusScoreRate();
    float plusScoreValue = blockStatus_->GetBaseScoreValue() * blockStatus_->GetPlusScoreRate();

    float breakOffsetTime = 0.0f;

    switch (blocktype) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:
        scoreStatus->PlusScoreIncrement(plusScoreValue);
        scoreStatus->SetScoreChangeTime(0.0f);
        effectType_ = EffectType::SCORE;
        tempValue_  = plusScoreValue;
        /* if (!breakNormal->isPlaying()) {*/
        breakNormal->Play();
        
        break;

        ///---------------------------------------------
        /// Skull
        ///---------------------------------------------
    case BlockType::SKULL:
        scoreStatus->PlusScoreIncrement(plusScoreValue);
        scoreStatus->SetScoreChangeTime(0.0f);
        effectType_ = EffectType::SCORE;
        tempValue_  = plusScoreValue;
        /*  timerStatus->MinusTimer(timerDecrementValue);*/
        /* effectType_ = EffectType::MIMUSTIME;
         tempValue_  = timerDecrementValue;*/
        breakSkull->Play();

        break;

        ///---------------------------------------------
        /// Advance
        ///---------------------------------------------
    case BlockType::ADVANTAGE:
        if (!combiStatus) {
            break;
        }

        // 合計スコア計算用のソート
        rightBlocks_ = combiStatus->GetRightBlocksForCalucration(blockStatus_->GetRowNum(), blockStatus_->GetColumnNum());

        for (BlockStatus* status : rightBlocks_) {

            switch (status->GetBlockType()) {
            case BlockType::NORMAL:
                plusTimerValue += combiStatus->GetPlusValue();
                plusScoreValue += combiStatus->GetPlusScoreValue();
                break;
            case BlockType::ADVANTAGE:
                plusTimerValue *= combiStatus->GetPlusRate();
                plusScoreValue *= combiStatus->GetPlusScoreRate();
                break;
            case BlockType::SKULL:
                if (plusTimerValue >= 0) {
                    plusTimerValue = -plusTimerValue;
                    plusScoreValue = -plusScoreValue;
                }
                break;
            default:
                break;
            }
        }

        //再ソート(左から右の順番になるように)
        rightBlocks_ = combiStatus->SortBlocksLeftToRight(rightBlocks_);

        for (BlockStatus* status : rightBlocks_) {
            // 破壊までのオフセットタイム
            breakOffsetTime += combiStatus->GetBreakOffsetTime();
            status->SetBreakOffsetTime(breakOffsetTime);
            status->SetIsBreakForAdvntageEffect(true);
            // 計算結果保持
            status->SetResultScore(plusScoreValue);
            status->SetResultTimer(plusTimerValue);
        }

        // スコア、タイマー加算
        if (plusTimerValue >= 0.0f) {
            timerStatus->TimerIncrement(plusTimerValue);
        } else {
            timerStatus->MinusTimer(-plusTimerValue);
        }
        scoreStatus->PlusScoreIncrement(plusScoreValue);
        scoreStatus->SetScoreChangeTime(0.0f);

        breakAdvance->Play();
        return;
        break;

    default:
        break;
    }

    SpawnerStatus->EffectUISpawn(Vec3f(blockTransform->worldMat[3]), tempValue_, effectType_);
}

void BreakBlockSystem::ScrapSpawn(GameEntity* _entity) {

    if (blockStatus_->GetIsNotScrapSpawn()) {
        return;
    }

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scrapEntity                  = ecsManager->getUniqueEntity("ScrapSpawner");

    if (!scrapEntity) {
        return;
    }

    ScrapSpawner* scrapSpawner = getComponent<ScrapSpawner>(scrapEntity);

    for (int32_t i = 0; i < scrapSpawner->GetCreateNum(); ++i) { // num分

        GameEntity* scrap = CreateEntity<Transform, Rigidbody, SphereCollider, ModelMeshRenderer, ScrapStatus>("Scrap", Transform(), Rigidbody(), SphereCollider(), ModelMeshRenderer(), ScrapStatus());

        // ================================= Componentを初期化 ================================= //

        //*Status
        ScrapStatus* status = getComponent<ScrapStatus>(scrap);
        status->SetFallStopPosY(scrapSpawner->GetFallStopPosY()); // stopPos
        status->SetLifeTime(scrapSpawner->GetLifeTime()); // lifeTime

        float blowValueX = 0.0f;
        float blowValueY = scrapSpawner->GetBlowValue()[Y];

        // 吹っ飛び向きを決める
        if (i % 2 == 0) {
            status->SetBlowDirection(BlowDirection::LEFT);
            blowValueX = -scrapSpawner->GetBlowValue()[X];
        } else {
            status->SetBlowDirection(BlowDirection::RIGHT);
            blowValueX = scrapSpawner->GetBlowValue()[X];
        }

        //* transform
        Transform* transform     = getComponent<Transform>(scrap);
        Transform* baseTransform = getComponent<Transform>(_entity);
        transform->translate     = baseTransform->translate;
        transform->translate[Z]  = 0.0f;

        //* rigitBody
        // 初速、重力、massの設定

        Rigidbody* rigitBody = getComponent<Rigidbody>(scrap);
        rigitBody->setMass(scrapSpawner->GetMass());
        rigitBody->setVelocity(Vec3f(blowValueX, blowValueY, 0.0f));
        rigitBody->setUseGravity(true);

        //* collider
        SphereCollider* collider              = getComponent<SphereCollider>(scrap);
        collider->getLocalShapePtr()->radius_ = scrapSpawner->GetColliderRadius();

        //*model
        ModelMeshRenderer* modelMesh = getComponent<ModelMeshRenderer>(scrap);
        CreateModelMeshRenderer(modelMesh, scrap, kApplicationResourceDirectory + "/Models/Scrap", "Scrap.obj");

        // ================================= System ================================= //
        ECSManager* ecs = ECSManager::getInstance();

        //------------------ Input
        // None

        //------------------ StateTransition
        ecs->getSystem<ScrapDeleteSystem>()->addEntity(scrap);
        //------------------ Movement
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(scrap);
        ecs->getSystem<ScrapFallSystem>()->addEntity(scrap);

        //------------------ Collision
        ecs->getSystem<CollisionCheckSystem>()->addEntity(scrap);
        ecs->getSystem<ScrapToPlayerCollisionSystem>()->addEntity(scrap);
        //------------------ Physics
        // None

        //------------------ Render
        /* ecs->getSystem<ColliderRenderingSystem>()->addEntity(block);*/
        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(scrap);
    }
}
