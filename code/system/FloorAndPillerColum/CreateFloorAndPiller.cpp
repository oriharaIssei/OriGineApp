#include "CreateFloorAndPiller.h"

/// app
// component
#include "component/FloorAndPillerColum/FloorAndPillerColum.h"
#include "component/Floor/FloorStates.h"
#include "component/Piller/PillerStates.h"

// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

// system

CreateFloorAndPiller::CreateFloorAndPiller() : ISystem(SystemType::Movement) {}
CreateFloorAndPiller::~CreateFloorAndPiller() {}

void CreateFloorAndPiller::Initialize() {}

void CreateFloorAndPiller::Finalize() {
  /*  entities_.clear();*/
}

void CreateFloorAndPiller::UpdateEntity(GameEntity* _entity) {
   
    FloorAndPillerColum* floorAndPillerColum = getComponent<FloorAndPillerColum>(_entity);
    floorAndPillerColum;
    //if (bulletSpawner == nullptr) {
    //    return;
    //}

    //// 弾を発射できなければ Skip
    //if (!bulletSpawner->getIsFire()) {
    //    return;
    //}

    //// 弾を発射
    //bulletSpawner->setIsFire(false);
    //bulletSpawner->setFireCoolTime(bulletSpawner->getMaxFireCoolTime());
    //SpawnBullet(_entity, bulletSpawner);
}

//void ShotBulletSystem::SpawnBullet(GameEntity* _entity, BulletSpawner* _spawner) {
//    // ================================= Bullet Entityを 生成 ================================= //
//    GameEntity* bullet = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer>("Bullet", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer());
//
//    // ================================= Componentを初期化 ================================= //
//    // Transform
//    Transform* hostTransfomr = getComponent<Transform>(_entity);
//    Transform* transform     = getComponent<Transform>(bullet);
//    transform->translate     = Vec3f(hostTransfomr->worldMat[3]) + _spawner->getBulletOffset();
//
//    // Collider
//    SphereCollider* collider           = getComponent<SphereCollider>(bullet);
//    collider->getLocalShape()->radius_ = _spawner->getBulletSize();
//
//    // Rigidbody
//    Rigidbody* rigidbody = getComponent<Rigidbody>(bullet);
//    rigidbody->setVelocity(Vec3f(0.0f, _spawner->getBulletSpeed(), 0.0f));
//
//    // MeshRenderer
//    ModelMeshRenderer* renderer = getComponent<ModelMeshRenderer>(bullet);
//    // Model から MeshRenderer を作成
//    CreateModelMeshRenderer(renderer, bullet, kApplicationResourceDirectory + "/Models", "sphere.obj");
//
//    // Status
//     CharacterStatus* status = getComponent<CharacterStatus>(bullet);
//     status->setHP(1);
//     status->setAttack(1);
//
//    // ================================= System ================================= //
//    ECSManager* ecs = ECSManager::getInstance();
//
//    ////------------------ Input
//    //// None
//
//    ////------------------ StateTransition
//    ecs->getSystem<DeleteCharacterEntitySystem>()->addEntity(bullet);
//
//    ////------------------ Movement
//    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bullet);
//
//    ////------------------ Collision
//    ecs->getSystem<CharacterOnCollision>()->addEntity(bullet);
//    ecs->getSystem<CollisionCheckSystem>()->addEntity(bullet);
//
//    ////------------------ Physics
//    //// None
//
//    ////------------------ Render
//    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bullet);
//}
