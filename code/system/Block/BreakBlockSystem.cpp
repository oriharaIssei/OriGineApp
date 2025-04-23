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
#include"system/EffectByBlock/EffectByBlockDeleteSystem.h"
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

    Transform* hostTransform = getComponent<Transform>(_entity);
    if (!hostTransform)
        return;

    const float effectValue = tempValue_;
    const EffectType type   = effectType_;

    int32_t intValue = static_cast<int32_t>(effectValue);
    int32_t absValue = std::abs(intValue);

    	///*　パラメータべた書きゾーン

    int32_t digitCount = (absValue == 0) ? 1 : static_cast<int32_t>(std::log10(absValue)) + 1;

    int32_t currentOffsetIndex = 0; // 数字用のオフセットインデックス

    for (int i = 0; i < digitCount + 2; ++i) {
        GameEntity* uiEntity = CreateEntity<Transform, Rigidbody, ModelMeshRenderer, EffectByBlockUIStatus>(
            "EffectUI", Transform(), Rigidbody(), ModelMeshRenderer(), EffectByBlockUIStatus());

        Transform* trans = getComponent<Transform>(uiEntity);
        trans->scale     = Vec3f(2.0f, 2.0f, 2.0f);

        EffectByBlockUIStatus* status = getComponent<EffectByBlockUIStatus>(uiEntity);
        ModelMeshRenderer* sprite     = getComponent<ModelMeshRenderer>(uiEntity);
        CreateModelMeshRenderer(sprite, uiEntity, kApplicationResourceDirectory + "/Models/Plane", "Plane.obj");

        switch (i) {
        case 0: // アイコン
            status->SetEffectType(type);
            status->SetCurerntIconTexture();
            status->SetDigit(UIDigit::ICON);
            trans->translate = Vec3f(hostTransform->worldMat[3]) + Vec3f(-5.0f, 0.0f, -6.0f); // 固定位置
            break;
        case 1: // 符号
            status->SetEffectType(type);
            status->SetCurerntSignTexture();
            status->SetDigit(UIDigit::SIGN);
            trans->translate = Vec3f(hostTransform->worldMat[3]) + Vec3f(-2.5f, 0.0f, -6.0f); // 固定位置
            break;
        default: // 数字
            status->SetEffectType(type);
            status->SetValue(effectValue);
            status->SetDigit(static_cast<UIDigit>(i));
            int32_t ditinum = status->GetValueForDigit();
            status->SetCurerntNumberTexture(ditinum);

            // 数字のみオフセットを加算
            trans->translate = Vec3f(hostTransform->worldMat[3]) + Vec3f(static_cast<float>((-2.5f * currentOffsetIndex) + 3.0f), 0.0f, -6.0f);
            ++currentOffsetIndex;
            break;
        }

        std::string textureName = status->GetCurrentTextureName();

        if (textureName.empty()) {
            trans->scale = Vec3f(0.0f, 0.0f, 0.0f);
            continue;
        }

        sprite->setTexture(0, kApplicationResourceDirectory + textureName);

        //status
        status->SetLifeTime(1.0f);

        //rigit body
        Rigidbody* rigit = getComponent<Rigidbody>(uiEntity);
        rigit->setVelocity(Vec3f(0.0f, 1.0f, 0.0f));

        ECSManager* ecs = ECSManager::getInstance();
        ecs->getSystem<EffectByBlockDeleteSystem>()->addEntity(uiEntity);
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(uiEntity);
        ecs->getSystem<EffectByBlockDrawSystem>()->addEntity(uiEntity);
        
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
