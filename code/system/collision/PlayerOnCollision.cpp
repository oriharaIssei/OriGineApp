#include "PlayerOnCollision.h"

/// ECS

// component
#include "component/transform/Transform.h"

#include "component/collision/CollisionPushBackInfo.h"
#include "component/physics/Rigidbody.h"

#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

#include "component/Stage/Stage.h"
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
    // 毎フレーム、地面・壁との衝突状態をリセット
    state->OffCollisionGround();
    state->OffCollisionWall();

    for (auto& [entityId, info] : pushBackInfo->GetCollisionInfoMap()) {
        Vec3f collNormal       = info.collVec.normalize();
        Entity* collidedEntity = GetEntity(entityId);

        // ゴール と 衝突したか
        if (collidedEntity->GetDataType().find("Goal") != std::string::npos) {
            Entity* timer = GetUniqueEntity("Timer");

            // クリア時間をセット
            if (timer) {
                Stage::SetClearTime(GetComponent<TimerComponent>(timer)->GetCurrentTime());
            }

            // ゴールと衝突した場合は、ゴールに到達したと判断する
            state->SetGoal(true);
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
}
