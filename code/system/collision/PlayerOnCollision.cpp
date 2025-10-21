#include "PlayerOnCollision.h"

/// ECS

// component
#include "component/transform/Transform.h"

#include "component/collision/CollisionPushBackInfo.h"
#include "component/physics/Rigidbody.h"

#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

#include "component/TimerComponent.h"

void PlayerOnCollision::Initialize() {
}

void PlayerOnCollision::Finalize() {
}

static const float kGroundCheckThreshold = 0.7f; // 地面と判断するための閾値
static const float kWallCheckThreshold   = 0.3f; // 壁と判断するための閾値

void PlayerOnCollision::UpdateEntity(Entity* _entity) {
    auto* state        = getComponent<PlayerState>(_entity);
    auto* pushBackInfo = getComponent<CollisionPushBackInfo>(_entity);
    auto* rigidbody    = getComponent<Rigidbody>(_entity);

    if (state == nullptr) {
        return;
    }
    state->OffCollisionGround();
    state->OffCollisionWall();

    for (auto& [entityId, info] : pushBackInfo->getCollisionInfoMap()) {
        Vec3f collNormal       = info.collVec.normalize();
        Entity* collidedEntity = getEntity(entityId);

        // ゴール と 衝突したか
        if (collidedEntity->getDataType().find("Goal") != std::string::npos) {
            // Entity* timer = getUniqueEntity("Timer");
            // Stage::setClearTime(getComponent<TimerComponent>(timer)->getCurrentTime());

            // ゴールと衝突した場合は、ゴールに到達したと判断する
            state->setGoal(true);
            continue;
        }

        if (collNormal[Y] > kGroundCheckThreshold) {
            // 上方向に衝突した場合は、地面にいると判断する
            state->OnCollisionGround(entityId);

            Vec3f acceleration = rigidbody->getAcceleration();

            // Y軸の加速度を0にする
            acceleration[Y] = 0.f;
            rigidbody->setAcceleration(acceleration);

            rigidbody->setVelocity(Y, 0.f);
        } else {
            // 壁と衝突した場合
            float dotVN = rigidbody->getVelocity().normalize().dot(collNormal);

            // どれくらい平行に動いているか (1.0 = 完全に平行, 0.0 = 完全に垂直)
            float parallelFactor = 1.f - std::fabs(dotVN);

            // 壁に沿って移動している場合は壁衝突とみなす
            if (parallelFactor > kWallCheckThreshold) {
                state->OnCollisionWall(collNormal, entityId);
            }
        }
    }
}
