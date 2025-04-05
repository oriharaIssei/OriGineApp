#include "DeleteBomSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Bom/BomStatus.h"
#include "component/Bom/ExplotionCollision.h"
#include "engine/EngineInclude.h"

DeleteBomSystem::DeleteBomSystem() : ISystem(SystemType::StateTransition) {}

void DeleteBomSystem::Initialize() {
}

void DeleteBomSystem::Finalize() {
    /*  entities_.clear();*/
    /*  if (status->GetCurrentTimer() >= status->GetExplotionTime()) {*/
}

DeleteBomSystem::~DeleteBomSystem() {}

void DeleteBomSystem::UpdateEntity(GameEntity* _entity) {
    auto status       = getComponent<BomStatus>(_entity);
    auto addcollision = getComponent<ExplotionCollision>(_entity);

    if (status->GetIsExplotion()) {
        AddExplotionEntity(_entity, addcollision); // コリジョン追加
        DestroyEntity(_entity); // 君消す
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
    /* ecs->getSystem<DeleteCharacterEntitySystem>()->addEntity(bullet);*/

    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bomCollision);

    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(bomCollision);
    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bomCollision);
}
