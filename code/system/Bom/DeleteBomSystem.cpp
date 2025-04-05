#include "DeleteBomSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// component
#include "component/Bom/BomStatus.h"
#include"component/Player/PlayerStates.h"
#include "component/Bom/ExplotionCollision.h"

#include "system/Bom/BomExplotionSystem.h"

DeleteBomSystem::DeleteBomSystem() : ISystem(SystemType::StateTransition) {}

void DeleteBomSystem::Initialize() {
}

void DeleteBomSystem::Finalize() {
    /*  entities_.clear();*/
    /*  if (status->GetCurrentTimer() >= status->GetExplotionTime()) {*/
}

DeleteBomSystem::~DeleteBomSystem() {}

void DeleteBomSystem::UpdateEntity(GameEntity* _entity) {
    BomStatus* status                = getComponent<BomStatus>(_entity);
    ExplotionCollision* addcollision = getComponent<ExplotionCollision>(_entity);

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* playerEntity                 = ecsManager->getUniqueEntity("Player");

    if (!status || !addcollision || !playerEntity) {
        return;
    }

     PlayerStates* playerStates = getComponent<PlayerStates>(playerEntity);

    if (status->GetIsExplotion()) {
        AddExplotionEntity(_entity, addcollision); // コリジョン追加
        DestroyEntity(_entity); // 君消す
        playerStates->SetincrementBomExplotionNum();
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
    bomTransform->translate  = Vec3f(hostTransform->worldMat[3]) + Vec3f(_bomStates->GetPositionOffset());

    // Collider
    SphereCollider* collider           = getComponent<SphereCollider>(bomCollision);
    collider->getLocalShape()->radius_ = _bomStates->GetCollisionRadius();

    /// States
    ExplotionCollision* status = getComponent<ExplotionCollision>(bomCollision);
    status                     = _bomStates;

    // ================================= System ================================= //
    ECSManager* ecs = ECSManager::getInstance();
    //------------------ Input
    // None

    //------------------ StateTransition
    ecs->getSystem<BomExplotionSystem>()->addEntity(bomCollision);

    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bomCollision);

    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(bomCollision);
    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bomCollision);
}
