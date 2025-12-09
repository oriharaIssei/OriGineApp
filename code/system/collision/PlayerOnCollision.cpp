#include "PlayerOnCollision.h"

/// ECS
// component
#include "component/collision/CollisionPushBackInfo.h"
#include "component/physics/Rigidbody.h"

#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

#include "component/TimerComponent.h"

void PlayerOnCollision::Initialize() {}
void PlayerOnCollision::Finalize() {}

static const float kGroundCheckThreshold     = 0.7f; // 地面と判断するための閾値
static const float kWallCheckThreshold       = 0.43f; // 壁と判断するための閾値
static const float kParallelPenaltyThreshold = 0.07f; // 障害物と判断するための閾値

constexpr float kPenaltyTime = 1.2f;

void PlayerOnCollision::UpdateEntity(Entity* _entity) {
    auto* state        = GetComponent<PlayerState>(_entity);
    auto* pushBackInfo = GetComponent<CollisionPushBackInfo>(_entity);
    auto* rigidbody    = GetComponent<Rigidbody>(_entity);

    if (state == nullptr) {
        return;
    }

    bool isPenalty            = false;
    float penaltyTime         = 0.f;
    Vec3f penaltyObjectNormal = Vec3f(0.f, 0.f, 0.f);

    // 毎フレーム、地面・壁との衝突状態をリセット
    state->OffCollisionGround();
    state->OffCollisionWall();

    for (auto& [entityId, info] : pushBackInfo->GetCollisionInfoMap()) {
        Entity* collEnt = GetEntity(entityId);
        // ゴール と 衝突したか
        if (collEnt->GetDataType().find("Goal") != std::string::npos) {
            // 時間を更新しないようにする
            Entity* timerEntity = GetUniqueEntity("Timer");
            if (timerEntity) {
                auto* timer = GetComponent<TimerComponent>(timerEntity);
                timer->SetStarted(false);
            }

            // ゴールと衝突した場合は、ゴールに到達したと判断する
            state->GetStateFlagRef().CurrentRef().SetFlag(PlayerStateFlag::IS_GOAL);
            continue;
        }

        // 壁、床と 衝突したか
        if (info.pushBackType != CollisionPushBackType::PushBack) {
            continue;
        }

        Vec3f collNormal = info.collFaceNormal.normalize();

        float absCollNXZ = std::abs(collNormal[X]) + std::abs(collNormal[Z]);

        if (collNormal[Y] > kGroundCheckThreshold) {
            // 上方向に衝突した場合は、地面にいると判断する
            state->OnCollisionGround();

            Vec3f acceleration = rigidbody->GetAcceleration();

            // Y軸の加速度を0にする
            acceleration[Y] = 0.f;
            rigidbody->SetAcceleration(acceleration);

            rigidbody->SetVelocity(Y, 0.f);
        } else if (absCollNXZ > kGroundCheckThreshold) {
            // 壁と衝突した場合
            float dotVN = rigidbody->GetVelocity().normalize().dot(collNormal);

            // どれくらい平行に動いているか (1.0 = 完全に平行, 0.0 = 完全に垂直)
            float parallelFactor = 1.f - std::fabs(dotVN);

            // 壁に沿って移動している場合は壁衝突とみなす
            // 正面衝突の場合はペナルティを与える
            if (parallelFactor > kWallCheckThreshold) {
                state->OnCollisionWall(collNormal, entityId);
            } else if (parallelFactor < kParallelPenaltyThreshold) { // 基準値以下なら ペナルティ
                if (!isPenalty) {
                    isPenalty           = true;
                    penaltyTime         = kPenaltyTime;
                    penaltyObjectNormal = collNormal;
                }
            }
        }
    }

    // ペナルティ状態を更新
    if (isPenalty) {
        PlayerStatus* status = GetComponent<PlayerStatus>(_entity);
        state->OnCollisionObstacle(penaltyTime, status->GetInvincibilityTime());

        if (state->IsPenalty()) {
            // 壁ジャンプの反動を与える
            constexpr float kReflectedSpeed = 48.f;
            Vec3f currentVelocity           = rigidbody->GetVelocity();
            currentVelocity                 = Reflect<float>(currentVelocity, penaltyObjectNormal);
            currentVelocity                 = currentVelocity.normalize() * std::max(kReflectedSpeed, currentVelocity.length() * 0.7f);
            rigidbody->SetVelocity(currentVelocity);
        }
    }
}
