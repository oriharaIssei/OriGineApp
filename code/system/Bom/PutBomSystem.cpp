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
#include"component/Bom/BomStatus.h"
#include"component/Bom/ExplotionCollision.h"

// system
#include"system/Bom/BomExplotionSystem.h"
#include"system/Bom/DeleteBomSystem.h"
#include"system/Bom/DeleteExplotionCollision.h"

void PutBomSystem::Initialize() {}

void PutBomSystem::Finalize() {
    entities_.clear();
}

void PutBomSystem::UpdateEntity(GameEntity* _entity) {

    BomSpawner* bomSpawner = getComponent<BomSpawner>(_entity);
    BomStatus* bomStates   = getComponent<BomStatus>(_entity);

    if (!bomSpawner || !bomStates) {
        return;
    }

    // 弾を発射できなければ Skip
    if (!bomSpawner->GetIsPut()) {
        return;
    }

    // 弾を発射
    bomSpawner->SetIsPut(false);
    bomSpawner->SetPutCurrentCoolTime(bomSpawner->GetPutCoolTimeMax());
    bomSpawner->SetIncrementPuttingNum();
    SpawnBom(_entity, bomStates);
}

void PutBomSystem::SpawnBom(GameEntity* _entity, BomStatus* _status) {
    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* bom = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer, BomStatus, ExplotionCollision>("Bom", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), BomStatus(), ExplotionCollision());

    // ================================= Componentを初期化 ================================= //

    // Transform
    Transform* hostTransform = getComponent<Transform>(_entity); // 設置元
    Transform* bomTransform  = getComponent<Transform>(bom);     // ボム
    bomTransform->translate  = Vec3f(hostTransform->worldMat[3]) + _status->GetPositionOffset();

    // Collider
    SphereCollider* collider = getComponent<SphereCollider>(bom);
    collider->getLocalShape()->radius_ = _status->GetCollisionRadius();

    //RigitBody
   /* Rigidbody* rigidbody = getComponent<Rigidbody>(bom);*/

     // MeshRenderer
    ModelMeshRenderer* renderer = getComponent<ModelMeshRenderer>(bom);
    // Model から MeshRenderer を作成
    CreateModelMeshRenderer(renderer, bom, kApplicationResourceDirectory + "/Models/Bom","Bom.obj");

    /// States
    BomStatus* status = getComponent<BomStatus>(bom);
    ExplotionCollision* collisionState = getComponent<ExplotionCollision>(bom);
    status            = _status;
    collisionState->SetAdaptTime(0.5f);
    collisionState->SetCollisionRadius(5.0f);
    collisionState->SetOffset(Vec3f(0.0f, 0.0f, 0.0f));

    // ================================= System ================================= //
    ECSManager* ecs = ECSManager::getInstance();
     //------------------ Input
    // None

    //------------------ StateTransition
    ecs->getSystem<DeleteBomSystem>()->addEntity(bom);
    ecs->getSystem<DeleteExplotionCollision>()->addEntity(bom);
    

    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bom);
    ecs->getSystem<BomExplotionSystem>()->addEntity(bom);

    //------------------ Collision
  /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/
    ecs->getSystem<CollisionCheckSystem>()->addEntity(bom);

    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bom);
}
