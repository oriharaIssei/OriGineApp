#include "DeleteBigBomSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// component
#include "component/BigBom/BigBomStatus.h"
#include"component/Player/PlayerStates.h"
#include "component/BigBom/BigExplotionCollision.h"

#include "system/Bom/BomExplotionSystem.h"
#include"system/BigBom/DeleteBigExplotionCollision.h"

DeleteBigBomSystem::DeleteBigBomSystem() : ISystem(SystemType::StateTransition) {}

void DeleteBigBomSystem::Initialize() {
}

void DeleteBigBomSystem::Finalize() {
    /*  entities_.clear();*/
    /*  if (status->GetCurrentTimer() >= status->GetExplotionTime()) {*/
}

DeleteBigBomSystem::~DeleteBigBomSystem() {}

void DeleteBigBomSystem::UpdateEntity(GameEntity* _entity) {
    BigBomStatus* status = getComponent<BigBomStatus>(_entity);
  

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* playerEntity                 = ecsManager->getUniqueEntity("Player");
    BigExplotionCollision* bigexCollision = getComponent<BigExplotionCollision>(playerEntity);

    if (!status || !bigexCollision) {
        return;
    }

   

    if (status->GetIsExplotion()) {
        AddExplotionEntity(_entity, bigexCollision); // コリジョン追加
        DestroyEntity(_entity); // 
 
    }
}

void DeleteBigBomSystem::AddExplotionEntity(GameEntity* _entity, BigExplotionCollision* _bomStates) {
    if (!_bomStates) {
        return;
    }

    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* bomCollision = CreateEntity<Transform, SphereCollider, Rigidbody, BigExplotionCollision>("BigColliExp", Transform(), SphereCollider(), Rigidbody(), BigExplotionCollision());

    // ================================= Componentを初期化 ================================= //

    // Transform
    Transform* hostTransform = getComponent<Transform>(_entity); // 設置元
    Transform* bomTransform  = getComponent<Transform>(bomCollision); // ボム
    bomTransform->translate  = Vec3f(hostTransform->worldMat[3]);

    // Collider
    SphereCollider* collider           = getComponent<SphereCollider>(bomCollision);
    collider->getLocalShapePtr()->radius_ = _bomStates->GetCollisionRadius();
   /* collider->setActive(true);*/
   /* collider->getWorldShape()->radius_ = _bomStates->GetCollisionRadius();*/

    /// States
    BigExplotionCollision* status = getComponent<BigExplotionCollision>(bomCollision);
    status                     = _bomStates;

    // ================================= System ================================= //
    ECSManager* ecs = ECSManager::getInstance();
    //------------------ Input
    // None

    //------------------ StateTransition
  /*  ecs->getSystem<BomExplotionSystem>()->addEntity(bomCollision);*/
    ecs->getSystem<DeleteBigExplotionCollision>()->addEntity(bomCollision);

    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bomCollision);

    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(bomCollision);
    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bomCollision);
}
