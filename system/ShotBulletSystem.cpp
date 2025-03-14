#include "ShotBulletSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "EngineInclude.h"

#include "component/BulletSpawner.h"

void ShotBulletSystem::Initialize() {}

void ShotBulletSystem::Finalize() {
    entities_.clear();
}

void ShotBulletSystem::UpdateEntity(GameEntity* _entity) {
    BulletSpawner* bulletSpawner = getComponent<BulletSpawner>(_entity);

    if (bulletSpawner == nullptr) {
        return;
    }

    // 弾を発射できなければ Skip
    if (!bulletSpawner->getIsFire()) {
        return;
    }

    // 弾を発射
    bulletSpawner->setIsFire(false);
    bulletSpawner->setFireCoolTime(bulletSpawner->getMaxFireCoolTime());
    SpawnBullet(_entity, bulletSpawner);
}

void ShotBulletSystem::SpawnBullet(GameEntity* _entity, BulletSpawner* _spawner) {
    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* bullet = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer>("Bullet", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer());

    // ================================= Componentを初期化 ================================= //
    // Transform
    Transform* hostTransfomr = getComponent<Transform>(_entity);
    Transform* transform     = getComponent<Transform>(bullet);
    transform->translate     = Vec3f(hostTransfomr->worldMat[3]) + _spawner->getBulletOffset();

    // Collider
    SphereCollider* collider           = getComponent<SphereCollider>(bullet);
    collider->getLocalShape()->radius_ = _spawner->getBulletSize();

    // Rigidbody
    Rigidbody* rigidbody = getComponent<Rigidbody>(bullet);
    rigidbody->setVelocity(Vec3f(0.0f, _spawner->getBulletSpeed(), 0.0f));

    // MeshRenderer
    ModelMeshRenderer* renderer = getComponent<ModelMeshRenderer>(bullet);
    // Model から MeshRenderer を作成
    CreateModelMeshRenderer(renderer, bullet, kApplicationResourceDirectory + "/Models", "sphere.obj");

    // ================================= System ================================= //
}
