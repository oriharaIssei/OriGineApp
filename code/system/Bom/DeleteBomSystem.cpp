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
#include "component/OperateUI/OperateUIStatus.h"

#include "system/Bom/BomExplotionSystem.h"
#include "system/Bom/DeleteExplotionCollision.h"

DeleteBomSystem::DeleteBomSystem() : ISystem(SystemType::StateTransition) {}

void DeleteBomSystem::Initialize() {
}

void DeleteBomSystem::Finalize() {
    /*  entities_.clear();*/
    /*  if (status->GetCurrentTimer() >= status->GetExplotionTime()) {*/
}

DeleteBomSystem::~DeleteBomSystem() {}

void DeleteBomSystem::UpdateEntity(GameEntity* _entity) {
    BomStatus* status = getComponent<BomStatus>(_entity);
    Transform* transform = getComponent<Transform>(_entity);

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* playerEntity                 = ecsManager->getUniqueEntity("Player");
    GameEntity* operateUI                    = ecsManager->getUniqueEntity("OperateUI");

    ExplotionCollision* addcollision = getComponent<ExplotionCollision>(playerEntity);
    OperateUIStatus* operateUIStatus = getComponent<OperateUIStatus>(operateUI);

    Audio* audio = getComponent<Audio>(playerEntity);
   

    if (!status || !addcollision || !playerEntity || !operateUIStatus || !audio) {
        return;
    }

    BomSpawner* bomSpawner = getComponent<BomSpawner>(playerEntity);

    if (status->GetIsExplotion() || transform->translate[Y]>50.0f) {
        AddExplotionEntity(_entity, addcollision); // コリジョン追加
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
