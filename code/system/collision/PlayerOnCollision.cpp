#include "PlayerOnCollision.h"

/// ECS

// component
#include "component/transform/Transform.h"

#include "component/collider/CollisionPushBackInfo.h"
#include "component/physics/Rigidbody.h"

#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

#include "component/TimerComponent.h"

void PlayerOnCollision::Initialize() {
}

void PlayerOnCollision::Finalize() {
}

static const float GROUND_CHECK_THRESHOLD = 0.7f; // 地面と判断するための閾値
// static const float WALL_CHECK_THRESHOLD   = 0.7f; // 壁と判断するための閾値

void PlayerOnCollision::UpdateEntity(GameEntity* _entity) {
    auto* state        = getComponent<PlayerState>(_entity);
    auto* pushBackInfo = getComponent<CollisionPushBackInfo>(_entity);

    if (state == nullptr) {
        return;
    }
    state->OffCollisionGround();
    state->OffCollisionWall();

    for (auto& [entityId, info] : pushBackInfo->getCollisionInfoMap()) {
        Vec3f collNormal           = info.collVec.normalize();
        GameEntity* collidedEntity = getEntity(entityId);

        // ゴール と 衝突したか
        if (collidedEntity->getDataType().find("Goal") != std::string::npos) {
            // GameEntity* timer = getUniqueEntity("Timer");
            // Stage::setClearTime(getComponent<TimerComponent>(timer)->getCurrentTime());

            // ゴールと衝突した場合は、ゴールに到達したと判断する
            state->setGoal(true);
            continue;
        }

        if (collNormal[Y] > GROUND_CHECK_THRESHOLD) {
            // 上方向に衝突した場合は、地面にいると判断する
            state->OnCollisionGround(entityId);

            auto* rigidbody    = getComponent<Rigidbody>(_entity);
            Vec3f acceleration = rigidbody->getAcceleration();

            // Y軸の加速度を0にする
            acceleration[Y] = 0.f;
            rigidbody->setAcceleration(acceleration);

            rigidbody->setVelocity(Y, 0.f);
        } else {
            Vec3f localNormal = collNormal;

            Transform* transform = getComponent<Transform>(collidedEntity);
            if (transform) {
                localNormal = localNormal * MakeMatrix::RotateQuaternion(transform->rotate).inverse();
            }

            state->OnCollisionWall(collNormal, entityId);
        }
    }
}
