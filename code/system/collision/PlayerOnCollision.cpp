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

void PlayerOnCollision::UpdateEntity(GameEntity* _entity) {
    auto* status       = getComponent<PlayerStatus>(_entity);
    auto* pushBackInfo = getComponent<CollisionPushBackInfo>(_entity);

    if (status == nullptr) {
        return;
    }
    status->setOnGround(false);

    for (auto& [entity, info] : pushBackInfo->getCollisionInfoMap()) {
        if (info.collVec.normalize()[Y] > GROUND_CHECK_THRESHOLD) {
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
        }
    }
}
