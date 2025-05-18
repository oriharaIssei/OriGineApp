#include "PutBomSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

/// app
// component
#include "component/Bom/BomSpawner.h"
#include "component/Bom/BomStatus.h"
#include "component/Bom/ExplotionCollision.h"
#include"component/OperateUI/OperateUIStatus.h"

// system
#include "system/Bom/BomCollisionExSystem.h"
#include "system/Bom/BomExplotionSystem.h"
#include "system/Bom/DeleteBomSystem.h"
#include "system/Bom/DeleteExplotionCollision.h"


void PutBomSystem::Initialize() {}

void PutBomSystem::Finalize() {
    entities_.clear();
}

void PutBomSystem::UpdateEntity(GameEntity* _entity) {

    bomSpawner_          = getComponent<BomSpawner>(_entity);
    bomExCollision_      = getComponent<ExplotionCollision>(_entity);
    BomStatus* bomStates = getComponent<BomStatus>(_entity);

    if (!bomSpawner_ || !bomStates || !bomExCollision_) {
        return;
    }
    if (!bomSpawner_->GetIsLaunch()) {
        return;
    }

    // 弾を発射  
    SpawnBom(_entity, bomStates);
    bomSpawner_->SetIsLaunch(false);
    bomSpawner_->SetIsLaunched(true);

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* operateUI           = ecsManager->getUniqueEntity("OperateUI");

    if (!operateUI) {
        return;
    }

    // 発射音
    Audio* launchSound = getComponent<Audio>(_entity, 1);
    launchSound->Play();

    OperateUIStatus* operateUIStatus = getComponent<OperateUIStatus>(operateUI);
    operateUIStatus->ChangeInit(OperateMode::EXPLOTION);
}

void PutBomSystem::SpawnBom(GameEntity* _entity, BomStatus* _status) {

    if (bomSpawner_->GetIsLaunched()) {
        return;
    }

    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* bom = CreateEntity<Transform, SphereCollider, SphereCollider, Rigidbody, ModelMeshRenderer, BomStatus>("Bom", Transform(), SphereCollider(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), BomStatus());
    //  ================================= Componentを初期化 ================================= //

    // Transform
    Transform* hostTransform = getComponent<Transform>(_entity); // 設置元
    Transform* bomTransform  = getComponent<Transform>(bom); // ボム
    bomTransform->translate  = Vec3f(hostTransform->worldMat[3]) + _status->GetPositionOffset();
    bomTransform->scale      = Vec3f(3.0f, 3.0f, 3.0f);

    // Collider
    SphereCollider* collider              = getComponent<SphereCollider>(bom);
    SphereCollider* collider2              = getComponent<SphereCollider>(bom,1);
    collider->getLocalShapePtr()->radius_ = _status->GetCollisionRadius();
    collider2->getLocalShapePtr()->radius_ = bomExCollision_->GetCollisionRadius();

    // RigitBody

    // MeshRenderer
    ModelMeshRenderer* renderer = getComponent<ModelMeshRenderer>(bom);
    // Model から MeshRenderer を作成
    CreateModelMeshRenderer(renderer, bom, kApplicationResourceDirectory + "/Models/Bomb", "Bomb.obj");

    /// States
    BomStatus* status = getComponent<BomStatus>(bom);
    status->SetBomNumber(bomSpawner_->GetPutTotalNum());
    status = _status;
    /*  ExplotionCollision* collisionState = getComponent<ExplotionCollision>(bom);*/

    // ================================= System ================================= //
    ECSManager* ecs = ECSManager::getInstance();
    //------------------ Input

    ecs->getSystem<BomExplotionSystem>()->addEntity(bom);
    //------------------ StateTransition
    ecs->getSystem<DeleteBomSystem>()->addEntity(bom);

    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bom);

    //------------------ Collision
    /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/
    ecs->getSystem<BomCollisionExSystem>()->addEntity(bom);
    ecs->getSystem<CollisionCheckSystem>()->addEntity(bom);

    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bom);
}
