#include "DeleteBomSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// component
#include "component/Bom/BomSpawner.h"
#include "component/Bom/BomStatus.h"
#include "component/Bom/ExplotionCollision.h"
#include "component/DeleteEntityStatus/DeleteEntityStatus.h"
#include "component/OperateUI/OperateUIStatus.h"

#include "system/Bom/BomExplotionSystem.h"
#include "system/Bom/DeleteExplotionCollision.h"
#include"system/DeleteEntitySystem/DeleteEntitySystem.h"

DeleteBomSystem::DeleteBomSystem() : ISystem(SystemType::StateTransition) {}

void DeleteBomSystem::Initialize() {
}

void DeleteBomSystem::Finalize() {
    /*  entities_.clear();*/
    /*  if (status->GetCurrentTimer() >= status->GetExplotionTime()) {*/
}

DeleteBomSystem::~DeleteBomSystem() {}

void DeleteBomSystem::UpdateEntity(GameEntity* _entity) {
    BomStatus* status    = getComponent<BomStatus>(_entity);
    Transform* transform = getComponent<Transform>(_entity);

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* playerEntity                 = ecsManager->getUniqueEntity("Player");
    GameEntity* operateUI                    = ecsManager->getUniqueEntity("OperateUI");

    ExplotionCollision* addcollision = getComponent<ExplotionCollision>(playerEntity);
    OperateUIStatus* operateUIStatus = getComponent<OperateUIStatus>(operateUI);

    Audio* audio = getComponent<Audio>(playerEntity, 0);

    if (!status || !addcollision || !playerEntity || !operateUIStatus || !audio) {
        return;
    }

    BomSpawner* bomSpawner = getComponent<BomSpawner>(playerEntity);

    if (status->GetIsExplotion() || transform->translate[Y] > 50.0f) {
        AddExplotionEntity(_entity, addcollision); // コリジョン追加
        CreateEffect(_entity);
        operateUIStatus->ChangeInit(OperateMode::LAUNCH);
        audio->Play();
        DestroyEntity(_entity); // 君消す
        bomSpawner->SetIsLaunched(false);
        bomSpawner->SetPutCurrentCoolTime(bomSpawner->GetPutCoolTimeMax());
    }
}

void DeleteBomSystem::AddExplotionEntity(GameEntity* _entity, ExplotionCollision* _bomStates) {
    if (!_bomStates) {
        return;
    }

    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* bomCollision = CreateEntity<Transform, SphereCollider, Rigidbody, ExplotionCollision>("ColliExp", Transform(), SphereCollider(), Rigidbody(), ExplotionCollision());

    // ================================= Componentを初期化 ================================= //

    // Transform
    Transform* hostTransform = getComponent<Transform>(_entity); // 設置元
    Transform* bomTransform  = getComponent<Transform>(bomCollision); // ボム
    bomTransform->translate  = Vec3f(hostTransform->worldMat[3]);

    // Collider
    SphereCollider* collider              = getComponent<SphereCollider>(bomCollision);
    collider->getLocalShapePtr()->radius_ = _bomStates->GetCollisionRadius();
    /* collider->getWorldShape()->radius_ = _bomStates->GetCollisionRadius();*/

    /// States
    ExplotionCollision* status = getComponent<ExplotionCollision>(bomCollision);
    status                     = _bomStates;

    // ================================= System ================================= //
    ECSManager* ecs = ECSManager::getInstance();
    //------------------ Input
    // None

    //------------------ StateTransition
    /*  ecs->getSystem<BomExplotionSystem>()->addEntity(bomCollision);*/
    ecs->getSystem<DeleteExplotionCollision>()->addEntity(bomCollision);

    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bomCollision);

    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(bomCollision);
    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bomCollision);
}

void DeleteBomSystem::CreateEffect(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* Emitentity                   = ecsManager->getUniqueEntity("BomEx");

    GameEntity* effect = CreateEntity<Emitter, Emitter, Emitter, Emitter, DeleteEntityStatus>("ExEffect", Emitter(), Emitter(), Emitter(), Emitter(), DeleteEntityStatus());

    DeleteEntityStatus* deleteEntityStatus = getComponent<DeleteEntityStatus>(effect);
    Transform* transform          = getComponent<Transform>(_entity);

    // 　生成済なら早期リターン(1回しか通らないはずなんだけど...)
    if (deleteEntityStatus->GetIsCreated()) {
        return;
    }
    deleteEntityStatus->SetDeleteTime(7.0f);

    // advantage
    for (int32_t i = 0; i < effect_.size(); ++i) {
        effect_[i] = getComponent<Emitter>(Emitentity, i);
    }

    /*for (int32_t i = 0; i < breakBlockEffects_.size(); ++i) {
        breakEffect_[i] = getComponent<Emitter>(blockbreakEffect, i);
    }*/

    // emitter
    Emitter* breakEffect0 = getComponent<Emitter>(effect, 0);
    Emitter* breakEffect1 = getComponent<Emitter>(effect, 1);
    Emitter* breakEffect2 = getComponent<Emitter>(effect, 2);
    Emitter* breakEffect3 = getComponent<Emitter>(effect, 3);

    // advantage
    nlohmann::json breakBlockEffects0 = *effect_[0];
    nlohmann::json breakBlockEffects1 = *effect_[1];
    nlohmann::json breakBlockEffects2 = *effect_[2];
    nlohmann::json breakBlockEffects3 = *effect_[3];

    *breakEffect0 = breakBlockEffects0;
    *breakEffect1 = breakBlockEffects1;
    *breakEffect2 = breakBlockEffects2;
    *breakEffect3 = breakBlockEffects3;

    // テクスチャを読み込む
    breakEffect0->Initialize(effect);
    breakEffect1->Initialize(effect);
    breakEffect2->Initialize(effect);
    breakEffect3->Initialize(effect);

    //// Particle発射
    Vec3f basePos = Vec3f(transform->worldMat[3]) + Vec3f(0.0f, 0.0f, -5.0f);

    ECSManager* ecs = ECSManager::getInstance();
    //------------------ Input
    // None

    //------------------ StateTransition
    ecs->getSystem<DeleteEntitySystem>()->addEntity(effect);
    //------------------ Movement
    ecs->getSystem<EmitterWorkSystem>()->addEntity(effect);
    //------------------ Collision

    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<ParticleRenderSystem>()->addEntity(effect);

    // set origin
    /* for (int32_t i = 0; i < breakBlockEffects_.size(); ++i) {
         breakEffect_[i]->setOriginePos(basePos);
         breakEffect_[i]->PlayStart();
     }*/

    // setorigin
    breakEffect0->setOriginePos(basePos);
    breakEffect1->setOriginePos(basePos);
    breakEffect2->setOriginePos(basePos);
    breakEffect3->setOriginePos(basePos);

    // shot
    breakEffect0->PlayStart();
    breakEffect1->PlayStart();
    breakEffect2->PlayStart();
    breakEffect3->PlayStart();

    // created
    deleteEntityStatus->SetIsCreated(true);
}
