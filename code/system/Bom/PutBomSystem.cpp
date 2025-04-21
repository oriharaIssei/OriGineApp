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

// system
#include "system/Bom/BomExplotionSystem.h"
#include "system/Bom/DeleteBomSystem.h"
#include "system/Bom/DeleteExplotionCollision.h"
#include"system/Bom/BomCollisionExSystem.h"

void PutBomSystem::Initialize() {}

void PutBomSystem::Finalize() {
    entities_.clear();
}

void PutBomSystem::UpdateEntity(GameEntity* _entity) {

    bomSpawner_          = getComponent<BomSpawner>(_entity);
    BomStatus* bomStates = getComponent<BomStatus>(_entity);

    if (!bomSpawner_ || !bomStates) {
        return;
    }

    // 弾を発射できなければ Skip
    if (!bomSpawner_->GetIsPut()) {
        return;
    }

    // 弾を発射
    bomSpawner_->SetIsPut(false);
    bomSpawner_->SetPutCurrentCoolTime(bomSpawner_->GetPutCoolTimeMax());
    SpawnBom(_entity, bomStates);
    bomSpawner_->SetIncrementTotalNum();
}

void PutBomSystem::SpawnBom(GameEntity* _entity, BomStatus* _status) {

    /* EntityComponentSystemManager* ecsManager = ECSManager::getInstance();*/
    /*("Bom" + std::to_string(bomSpawner_->GetPutTotalNum())).c_str()*/

    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* bom = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer, BomStatus>("Bom", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), BomStatus());
    //  ================================= Componentを初期化 ================================= //

    // Transform
    Transform* hostTransform = getComponent<Transform>(_entity); // 設置元
    Transform* bomTransform  = getComponent<Transform>(bom); // ボム
    bomTransform->translate  = Vec3f(hostTransform->worldMat[3]) + _status->GetPositionOffset();

    // Collider
    SphereCollider* collider           = getComponent<SphereCollider>(bom);
    collider->getLocalShapePtr()->radius_ = _status->GetCollisionRadius();

    // RigitBody
    /* Rigidbody* rigidbody = getComponent<Rigidbody>(bom);*/

    // MeshRenderer
    ModelMeshRenderer* renderer = getComponent<ModelMeshRenderer>(bom);
    // Model から MeshRenderer を作成
    CreateModelMeshRenderer(renderer, bom, kApplicationResourceDirectory + "/Models/Bom", "Bom.obj");

    /// States
    BomStatus* status = getComponent<BomStatus>(bom);
    status->SetBomNumber(bomSpawner_->GetPutTotalNum());
    status = _status;
  /*  ExplotionCollision* collisionState = getComponent<ExplotionCollision>(bom);*/

    //collisionState->SetAdaptTime(0.5f);
    //collisionState->SetCollisionRadius(5.0f);
    //collisionState->SetOffset(Vec3f(0.0f, 0.0f, 0.0f));

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
