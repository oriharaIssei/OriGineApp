#include "PlayerCreateBigBomSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include "component/BigBom/BigBomSpawner.h"
#include "component/BigBom/BigBomStatus.h"
#include "component/Player/PlayerStates.h"

#include <cmath>
#include <numbers>
#include <Quaternion.h>
#include <Vector3.h>

PlayerCreateBigBomSystem::PlayerCreateBigBomSystem()
    : ISystem(SystemType::Movement) {}

PlayerCreateBigBomSystem::~PlayerCreateBigBomSystem() {}

void PlayerCreateBigBomSystem::Initialize() {}
void PlayerCreateBigBomSystem::Finalize() {}

void PlayerCreateBigBomSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity)
        return;

    PlayerStates* playerStautus  = getComponent<PlayerStates>(_entity);
    BigBomSpawner* bigBomSpawner = getComponent<BigBomSpawner>(_entity);
    if (!playerStautus || !bigBomSpawner) {
        return;
    }

    // 持ってない＆ポイントで
    if (playerStautus->GetCurrentBigBomPoint() < playerStautus->GetBigBomPointMax()
        || playerStautus->GetIsBigBomHaving()) {
        return;
    }

    // プレイヤーの状態乞更新
    playerStautus->SetIsBigBomHaving(true);
    playerStautus->ReSetCurrentBigBomPoint();

    // BigBomを実際に生成


    GameEntity* bigBom = CreateEntity<Transform, Rigidbody, SphereCollider, ModelMeshRenderer, BigBomStatus>("bigBom", Transform(), Rigidbody(), SphereCollider(), ModelMeshRenderer(), BigBomStatus());

    // ================================= Componentを初期化 ================================= //
    //*Status
    BigBomStatus* status = getComponent<BigBomStatus>(bigBom);
    status->SetLaunchSpeed(bigBomSpawner->GetLaunghSpeed());
    status->SetOffset(bigBomSpawner->GetSpawnOffset());

    //* transform
    Transform* transform     = getComponent<Transform>(bigBom);
    Transform* baseTransform = getComponent<Transform>(_entity);
    transform->translate     = Vec3f(baseTransform->worldMat[3]) + status->GetOffset();

    //* rigitBody
    // 初速、重力、massの設定

    //* collider
    SphereCollider* collider              = getComponent<SphereCollider>(bigBom);
    collider->getLocalShapePtr()->radius_ = bigBomSpawner->GetCollisionRadius();
    collider->getLocalShapePtr()->center_ = bigBomSpawner->GetCollisionCenter();
    collider->setActive(false);

    //*model
    ModelMeshRenderer* modelMesh = getComponent<ModelMeshRenderer>(bigBom);
    CreateModelMeshRenderer(modelMesh, bigBom, kApplicationResourceDirectory + "/Models/Bom", "Bom.obj");

    // ================================= System ================================= //
    ECSManager* ecs = ECSManager::getInstance();

    //------------------ Input
    // None

    //------------------ StateTransition
   
    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bigBom);
   

    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(bigBom);
   
    //------------------ Physics
    // None

    //------------------ Render
    /* ecs->getSystem<ColliderRenderingSystem>()->addEntity(block);*/
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bigBom);
}
