#include "PlayerOnCollision.h"

/// ECS
#include "ECSManager.h"
// component
#include "component/collider/CollisionPushBackInfo.h"
#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerStatus.h"

void PlayerOnCollision::Initialize() {
}

void PlayerOnCollision::Finalize() {
}

static const float GROUND_CHECK_THRESHOLD = 0.7f; // 地面と判断するための閾値
static const float WALL_CHECK_THRESHOLD   = 0.7f; // 壁と判断するための閾値

void PlayerOnCollision::UpdateEntity(GameEntity* _entity) {
    auto* status       = getComponent<PlayerStatus>(_entity);
    auto* pushBackInfo = getComponent<CollisionPushBackInfo>(_entity);

    if (status == nullptr) {
        return;
    }
    status->setOnGround(false);
    status->setCollisionWithWall(false, {0.0f, 0.0f, 0.0f});

    for (auto& [entity, info] : pushBackInfo->getCollisionInfoMap()) {
        Vec3f collNormal = info.collVec.normalize();
        if (collNormal[Y] > GROUND_CHECK_THRESHOLD) {
            // 上方向に衝突した場合は、地面にいると判断する
            status->setOnGround(true);

            auto* rigidbody    = getComponent<Rigidbody>(_entity);
            Vec3f acceleration = rigidbody->getAcceleration();
            // Y軸の加速度を0にする
            acceleration[Y] = 0.f;
            rigidbody->setAcceleration(acceleration);

            Vec3f velo = rigidbody->getVelocity();
            // Y軸の速度を0にする
            velo[Y] = 0.f;
            rigidbody->setVelocity(velo);
        } else {
            if (std::abs(collNormal[X]) > WALL_CHECK_THRESHOLD) {
                // 壁と衝突した場合、地面にいると判断する
                status->setOnGround(true);
                status->setCollisionWithWall(true, collNormal);
            }
        }
    }
}
