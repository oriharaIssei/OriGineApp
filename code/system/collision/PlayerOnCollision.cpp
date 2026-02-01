#include "PlayerOnCollision.h"

/// ECS
// component
#include "component/collision/collider/SphereCollider.h"
#include "component/collision/CollisionPushBackInfo.h"
#include "component/physics/Rigidbody.h"

#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

#include "component/gimmick/RailPoints.h"

#include "component/TimerComponent.h"

/// math
#include "math/mathEnv.h"

using namespace OriGine;

void PlayerOnCollision::Initialize() {}
void PlayerOnCollision::Finalize() {}

static const float kGroundCheckThreshold = 0.7f; // 地面と判断するための閾値
static const float kWallCheckThreshold   = 0.42f; // 壁と判断するための閾値

void PlayerOnCollision::UpdateEntity(OriGine::EntityHandle _handle) {
    auto* state          = GetComponent<PlayerState>(_handle);
    auto* status         = GetComponent<PlayerStatus>(_handle);
    auto* pushBackInfo   = GetComponent<CollisionPushBackInfo>(_handle);
    auto* rigidbody      = GetComponent<Rigidbody>(_handle);
    auto* sphereCollider = GetComponent<SphereCollider>(_handle);

    if (state == nullptr || pushBackInfo == nullptr || rigidbody == nullptr || sphereCollider == nullptr) {
        return;
    }
    const auto& collisionStateMap = sphereCollider->GetCollisionStateMap();

    // 毎フレーム、地面・壁との衝突状態をリセット
    bool isPreWheelie             = state->IsWheelie(); // 前フレームのウィリー状態を保持
    EntityHandle wallEntityHandle = state->GetWallEntityIndex();
    state->OffCollisionGround();
    state->OffCollisionWall();
    state->OffCollisionRail();

    for (auto& [entityId, info] : pushBackInfo->GetCollisionInfoMap()) {
        OriGine::Entity* collEnt = GetEntity(entityId);
        // ゴール と 衝突したか
        if (collEnt->GetDataType().find("Goal") != std::string::npos) {
            // 時間を更新しないようにする
            OriGine::EntityHandle timerEntityHandle = GetUniqueEntity("Timer");
            auto* timer                             = GetComponent<TimerComponent>(timerEntityHandle);
            if (timer) {
                timer->SetStarted(false);
            }

            // ゴールと衝突した場合は、ゴールに到達したと判断する
            state->GetStateFlagRef().CurrentRef().SetFlag(PlayerStateFlag::IS_GOAL);
            continue;
        }

        // レールポイント と 衝突したか
        {
            RailPoints* railPoints = GetComponent<RailPoints>(entityId);
            if (railPoints && status->CanRideRail()) {
                state->OnCollisionRail(entityId);
                continue;
            }
        }
        // 壁、床と 衝突したか
        if (info.pushBackType != CollisionPushBackType::PushBack) {
            continue;
        }

        OriGine::Vec3f collNormal = info.collFaceNormal.normalize();

        float absCollNXZ = std::abs(collNormal[X]) + std::abs(collNormal[Z]);

        if (collNormal[Y] > kGroundCheckThreshold) {
            // 上方向に衝突した場合は、地面にいると判断する
            state->OnCollisionGround();

            status->ResetWallRunInterval();
            status->ResetWheelieInterval();

            OriGine::Vec3f acceleration = rigidbody->GetAcceleration();

            // Y軸の加速度を0にする
            acceleration[Y] = 0.f;
            rigidbody->SetAcceleration(acceleration);

            rigidbody->SetVelocity(Y, 0.f);
        } else if (absCollNXZ > kGroundCheckThreshold) {
            // 壁と衝突した場合
            float dotVN = rigidbody->GetVelocity().normalize().dot(collNormal);

            // どれくらい平行に動いているか (1.0 = 完全に平行, 0.0 = 完全に垂直)
            float parallelFactor = 1.f - std::fabs(dotVN);

            bool isFirstCollision = true;

            if (entityId == wallEntityHandle) {
                auto collisionStateItr = collisionStateMap.find(entityId);
                if (collisionStateItr != collisionStateMap.end()) {
                    CollisionState collState = collisionStateItr->second;
                    if (collState == CollisionState::Stay) {
                        isFirstCollision = false;
                    }
                }
            }

            if (isFirstCollision) {
                // 壁に沿って移動している場合は壁衝突とみなす
                // 正面衝突の場合はペナルティを与える
                if (parallelFactor > kWallCheckThreshold) {
                    if (status->CanWallRun()) {
                        state->OnCollisionWall(collNormal, entityId);
                    }
                } else { // 基準値以下なら
                    constexpr float kWheelieThresholdFallSpeed = -1.3f; // ウィリーになるための落下速度閾値
                    if (rigidbody->GetVelocity()[Y] > kWheelieThresholdFallSpeed) {
                        if (status->CanWheelie()) {
                            state->OnCollisionWall(collNormal, entityId, true);
                        }
                    }
                }
            } else {
                bool canContinue = false;
                canContinue      = isPreWheelie ? status->CanWheelie() : status->CanWallRun();
                if (canContinue) {
                    state->OnCollisionWall(collNormal, wallEntityHandle, isPreWheelie);
                }
            }
        }
    }
}
