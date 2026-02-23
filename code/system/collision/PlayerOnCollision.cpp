#include "PlayerOnCollision.h"

/// ECS
// component
#include "component/collision/collider/SphereCollider.h"
#include "component/collision/CollisionPushBackInfo.h"
#include "component/physics/Rigidbody.h"

#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

#include "component/gimmick/Obstacle.h"
#include "component/gimmick/RailPoints.h"
#include "component/gimmick/TimeScaleEffectComponent.h"

#include "component/TimerComponent.h"

#include "component/material/Material.h"

/// util
#include "component/player/PlayerMoveUtils.h"

/// math
#include "math/MathEnv.h"

using namespace OriGine;

void PlayerOnCollision::Initialize() {}
void PlayerOnCollision::Finalize() {}

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
    const auto& collisionInfoMap  = pushBackInfo->GetCollisionInfoMap();

    // 毎フレーム、地面・壁との衝突状態をリセット
    bool isPreWheelie             = state->IsWheelie(); // 前フレームのウィリー状態を保持
    EntityHandle wallEntityHandle = state->GetWallEntityIndex();
    state->OffCollisionGround();
    state->OffCollisionWall();
    state->OffCollisionRail();

    for (auto& [entityId, collisionState] : collisionStateMap) {
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
        // 障害物 と 衝突したか
        {
            Obstacle* obstacle = GetComponent<Obstacle>(entityId);
            if (obstacle) {
                float penaltyTime       = obstacle->GetPenaltyTime();
                float invincibilityTime = obstacle->GetInvincibilityTimeOnCollision();
                state->OnCollisionObstacle(penaltyTime, invincibilityTime);
                continue;
            }
        }

        // タイムスケールエフェクト と 衝突したか
        {
            TimeScaleEffectComponent* timeScaleEffect = GetComponent<TimeScaleEffectComponent>(entityId);
            if (timeScaleEffect) {
                // 当たり判定を消す。
                SphereCollider* collider = GetComponent<SphereCollider>(entityId);
                if (collider) {
                    collider->SetActive(false);
                }
                // scaleEffectを有効化
                timeScaleEffect->SetActive(true);

                Material* material = GetComponent<Material>(entityId);
                if (material) {
                    material->color_[A] = 0.f; // 透明にする
                }
            }
        }

        // 衝突情報を取得(壁の処理にはInfoがないとできない)
        auto infoItr = collisionInfoMap.find(entityId);
        if (infoItr == collisionInfoMap.end()) {
            continue;
        }

        const CollisionPushBackInfo::Info& info = infoItr->second;
        // 壁、床と 衝突したか
        if (info.pushBackType != CollisionPushBackType::PushBack) {
            continue;
        }

        OriGine::Vec3f collisionNormal = info.collFaceNormal.normalize();

        if (PlayerMoveUtils::IsHitGround(collisionNormal)) {
            // 上方向に衝突した場合は、地面にいると判断する
            state->OnCollisionGround();

            status->ResetWallRunInterval();
            status->ResetWheelieInterval();

            OriGine::Vec3f acceleration = rigidbody->GetAcceleration();

            // Y軸の加速度を0にする
            acceleration[Y] = 0.f;
            rigidbody->SetAcceleration(acceleration);

            rigidbody->SetVelocity(Y, 0.f);
        } else {
            // 壁と衝突した場合
            float dotVN = rigidbody->GetVelocity().normalize().dot(collisionNormal);

            // どれくらい平行に動いているか (1.0 = 完全に平行, 0.0 = 完全に垂直)
            float parallelFactor = 1.f - std::fabs(dotVN);

            bool isFirstCollision = true;

            if (entityId == wallEntityHandle) {
                auto collisionStateItr = collisionStateMap.find(entityId);
                if (collisionStateItr != collisionStateMap.end()) {
                    if (collisionStateItr->second == CollisionState::Stay) {
                        isFirstCollision = false;
                    }
                }
            }

            if (isFirstCollision) {
                PlayerMoveUtils::WallContactResult wallContactResult = PlayerMoveUtils::EvaluateWallContact(parallelFactor, rigidbody, status);
                switch (wallContactResult) {
                case PlayerMoveUtils::WallContactResult::WallRun:
                case PlayerMoveUtils::WallContactResult::Wheelie:
                    state->OnCollisionWall(collisionNormal, entityId, wallContactResult == PlayerMoveUtils::WallContactResult::Wheelie);
                    break;
                default:
                    break;
                }
            } else {
                // 前フレームから引き続き壁と接触している場合は、ウィリー状態を維持するかどうかを判断する
                state->OnCollisionWall(collisionNormal, wallEntityHandle, isPreWheelie);
            }
        }
    }
}
