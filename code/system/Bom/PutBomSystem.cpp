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
// system
//#include "system/CharacterOnCollision.h"
//#include "system/DeleteCharacterEntitySystem.h"

void PutBomSystem::Initialize() {}

void PutBomSystem::Finalize() {
    entities_.clear();
}

void PutBomSystem::UpdateEntity(GameEntity* _entity) {
    BomSpawner* bomSpawner = getComponent<BomSpawner>(_entity);
    BomStatus* bomStates   = getComponent<BomStatus>(_entity);

    if (bomSpawner == nullptr || bomStates==nullptr) {
        return;
    }

    // 弾を発射できなければ Skip
    if (!bomSpawner->GetIsPut()) {
        return;
    }

    // 弾を発射
    bomSpawner->SetIsPut(false);
    bomSpawner->SetPutCurrentCoolTime(bomSpawner->GetPutCoolTimeMax());
    SpawnBom(_entity, bomStates);
}

void PutBomSystem::SpawnBom(GameEntity* _entity, BomStatus* _status) {
    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* bom = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer,BomStatus>
        ("Bom", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(),BomStatus());

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
    status            = _status;

    // ================================= System ================================= //
    ECSManager* ecs = ECSManager::getInstance();
     //------------------ Input
    // None

    //------------------ StateTransition
   /* ecs->getSystem<DeleteCharacterEntitySystem>()->addEntity(bullet);*/

    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bom);

    //------------------ Collision
  /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/
    ecs->getSystem<CollisionCheckSystem>()->addEntity(bom);

    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bom);
}
