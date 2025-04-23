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
#include "component/EffectByBlock/EffectByBlockUIStatus.h"
#include "component/Score/ScoreStatus.h"
#include "component/Scrap/ScrapSpawner.h"
#include "component/Scrap/ScrapStatus.h"
#include "component/Timer/TimerStatus.h"
/// system
#include "system/scrap/ScrapDeleteSystem.h"
#include "system/scrap/ScrapFallSystem.h"
#include "system/scrap/ScrapToPlayerCollisionSystem.h"
#include"system/EffectByBlock/EffectByBlockDrawSystem.h"
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
        /* ScrapSpawn(_entity);*/
        BlockReaction(blockStatus_->GetBlockType());
        EffectUISpawn(_entity);
        DestroyEntity(_entity);
    }
}

void BreakBlockSystem::BlockReaction(BlockType blocktype) {

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

    float timerDecrementValue = timerStatus->GetPulusTime() * blockStatus_->GetRatio();
    float timerIncrementValue = timerStatus->GetMinusTime() * blockStatus_->GetRatio();
    float scoreValue          = blockStatus_->GetBaseScoreValue() * blockStatus_->GetRatio();

    switch (blocktype) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:
        scoreStatus->PlusScoreIncrement(scoreValue);
        scoreStatus->SetScoreChangeTime(0.0f);
        effectType_ = EffectType::SCORE;
        tempValue_  = scoreValue;
        break;

        ///---------------------------------------------
        /// Skull
        ///---------------------------------------------
    case BlockType::SKULL:
        timerStatus->TimerDecrement(timerDecrementValue);
        effectType_ = EffectType::MIMUSTIME;
        tempValue_  = timerDecrementValue;
        break;

        ///---------------------------------------------
        /// Advance
        ///---------------------------------------------
    case BlockType::ADVANTAGE:
        timerStatus->TimerIncrement(timerIncrementValue);
        effectType_ = EffectType::TIME;
        tempValue_  = timerIncrementValue;
        break;

    default:
        break;
    }
}

void BreakBlockSystem::EffectUISpawn(GameEntity* _entity) {
    if (!_entity)
        return;

   /* EntityComponentSystemManager* ecsManager = ECSManager::getInstance();*/

    Transform* hostTransform = getComponent<Transform>(_entity);
    if (!hostTransform)
        return;

    // 値とタイプを保持（BlockReactionで設定されたもの）
    const float effectValue = tempValue_;
    const EffectType type   = effectType_;

    // 数値を整数に変換（少数は不要なため丸め）
    int32_t intValue = static_cast<int32_t>(effectValue);
    int32_t absValue = std::abs(intValue);

    // 桁数を計算
    int32_t digitCount = 0;
    if (absValue == 0) {
        digitCount = 1;
    } else {
        digitCount = static_cast<int32_t>(std::log10(absValue)) + 1;
    }

    // ICON → SIGN → 数字 の順に生成
    for (int i = 0; i < digitCount; ++i) {
        GameEntity* uiEntity = CreateEntity<Transform, Rigidbody, SpriteRenderer, EffectByBlockUIStatus>(
            "EffectUI", Transform(), Rigidbody(), SpriteRenderer(), EffectByBlockUIStatus());

        Transform* trans = getComponent<Transform>(uiEntity);
        trans->translate = Vec3f(hostTransform->worldMat[3]) + Vec3f(static_cast<float>(3*i), 0.0f, 0.0f); // 横並びに配置

        EffectByBlockUIStatus* status = getComponent<EffectByBlockUIStatus>(uiEntity);

        SpriteRenderer* sprite = getComponent<SpriteRenderer>(uiEntity);

        switch (i) {
        case 0:
            status->SetEffectType(type);
            status->SetCurerntIconTexture();
            status->SetDigit(UIDigit::ICON);
            sprite->setTexture(kApplicationResourceDirectory +status->GetCurrentTextureName(), true);
            break;

        case 1:
            status->SetEffectType(type);
            status->SetCurerntSignTexture();
            status->SetDigit(UIDigit::SIGN);
            sprite->setTexture(kApplicationResourceDirectory+status->GetCurrentTextureName(), true);
            break;
        default:
            status->SetEffectType(type);
            status->SetCurerntNumberTexture();
            status->SetDigit(static_cast<UIDigit>(2 + i));
            int32_t ditinum = status->GetValueForDigit();

            sprite->setTexture(kApplicationResourceDirectory +status->GetCurrentTextureName(), false);
            sprite->setTextureSize(Vec2f(48.0f, 48.0f));
            sprite->setUVScale(Vec2f(0.1f, 1.0f));
            sprite->setUVTranslate(Vec2f(float(ditinum * 0.1f), 0.0f));
            break;
        }

        // ================================= System ================================= //
        ECSManager* ecs = ECSManager::getInstance();

        //------------------ Input
        // None

        //------------------ StateTransition
        //------------------ Movement
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(uiEntity);
        ecs->getSystem<EffectByBlockDrawSystem>()->addEntity(uiEntity);
     
      
        //------------------ Collision
        //------------------ Physics
        // None

        //------------------ Render
        /* ecs->getSystem<ColliderRenderingSystem>()->addEntity(block);*/
        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(uiEntity);
    }
}

void BreakBlockSystem::ScrapSpawn(GameEntity* _entity) {
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
        // status->SetPointValue(scrapSpawner->GetPointValue());    // pointValue
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
