#include "PlayerOnCollision.h"

/// ECS

// component
#include "component/collision/CollisionPushBackInfo.h"
#include "component/physics/Rigidbody.h"

#include "component/stage/StageObstacle.h"

#include "component/player/state/PlayerState.h"

#include "component/TimerComponent.h"

void PlayerOnCollision::Initialize() {
}

void PlayerOnCollision::Finalize() {
}

static const float kGroundCheckThreshold = 0.7f; // 地面と判断するための閾値
static const float kWallCheckThreshold   = 0.3f; // 壁と判断するための閾値

void PlayerOnCollision::UpdateEntity(Entity* _entity) {
    auto* state        = GetComponent<PlayerState>(_entity);
    auto* pushBackInfo = GetComponent<CollisionPushBackInfo>(_entity);
    auto* rigidbody    = GetComponent<Rigidbody>(_entity);

    if (state == nullptr) {
        return;
    }

    bool isPenalty = false;

    // 毎フレーム、地面・壁との衝突状態をリセット
    state->OffCollisionGround();
    state->OffCollisionWall();

    for (auto& [entityId, info] : pushBackInfo->GetCollisionInfoMap()) {
        Vec3f collNormal       = info.collVec.normalize();
        Entity* collidedEntity = GetEntity(entityId);

        // ゴール と 衝突したか
        if (collidedEntity->GetDataType().find("Goal") != std::string::npos) {
            // 時間を更新しないようにする
            auto* timer = GetComponent<TimerComponent>(_entity);
            timer->SetStarted(false);

            // ゴールと衝突した場合は、ゴールに到達したと判断する
            isPenalty = true;
            continue;
        }

        // 障害物 と 衝突したか
        StageObstacle* obstacle = GetComponent<StageObstacle>(collidedEntity);
        if (obstacle != nullptr) {
            // 障害物と衝突した場合は、ペナルティ時間を加算する
            auto* timer = GetComponent<TimerComponent>(_entity);
            timer->SetCurrentTime(timer->GetTime() + obstacle->GetPenaltyTime());
            isPenalty = true;

            continue;
        }

        if (collNormal[Y] > kGroundCheckThreshold) {
            // 上方向に衝突した場合は、地面にいると判断する
            state->OnCollisionGround(entityId);

            Vec3f acceleration = rigidbody->GetAcceleration();

            // Y軸の加速度を0にする
            acceleration[Y] = 0.f;
            rigidbody->SetAcceleration(acceleration);

            rigidbody->SetVelocity(Y, 0.f);
        } else if (std::abs(collNormal[X]) + std::abs(collNormal[Z]) > kGroundCheckThreshold) {
            // 壁と衝突した場合
            float dotVN = rigidbody->GetVelocity().normalize().dot(collNormal);

            // どれくらい平行に動いているか (1.0 = 完全に平行, 0.0 = 完全に垂直)
            float parallelFactor = 1.f - std::fabs(dotVN);

            // 壁に沿って移動している場合は壁衝突とみなす
            if (parallelFactor > kWallCheckThreshold) {
                state->OnCollisionWall(collNormal, entityId);
            }
        }
    }

    // ペナルティ状態を更新
    if (isPenalty) {
        auto& flag = state->GetStateFlagRef();
        flag.SetCurrent(flag.Current() | PlayerStateFlag::IS_PENALTY);
    }
}
