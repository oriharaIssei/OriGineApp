#include "PlayerAheadCollisionReactionSystem.h"

/// ECS
// component
#include "component/collision/collider/SphereCollider.h"
#include "component/collision/CollisionPushBackInfo.h"
#include "component/physics/Rigidbody.h"
#include "component/player/PlayerEffectControlParam.h"
#include "component/player/PlayerStatus.h"
#include "component/player/State/PlayerState.h"

/// util
#include "component/player/PlayerMoveUtils.h"

/// math
#include "math/mathEnv.h"

using namespace OriGine;

PlayerAheadCollisionReactionSystem::PlayerAheadCollisionReactionSystem() : ISystem(SystemCategory::Collision) {}

void PlayerAheadCollisionReactionSystem::Initialize() {}
void PlayerAheadCollisionReactionSystem::Finalize() {}

void PlayerAheadCollisionReactionSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    EntityHandle playerHandle                    = GetUniqueEntity("Player");
    PlayerState* playerState                     = GetComponent<PlayerState>(playerHandle);
    SphereCollider* playerCollider               = GetComponent<SphereCollider>(playerHandle);
    Rigidbody* playerRigidbody                   = GetComponent<Rigidbody>(playerHandle);
    PlayerStatus* playerStatus                   = GetComponent<PlayerStatus>(playerHandle);
    Transform* playerTransform                   = GetComponent<Transform>(playerHandle);
    PlayerEffectControlParam* effectControlParam = GetComponent<PlayerEffectControlParam>(playerHandle);

    if (!playerState || !playerStatus || !playerCollider || !playerRigidbody || !playerTransform || !effectControlParam) {
        LOG_WARN("PlayerState or Player SphereCollider component is not found. PlayerAheadCollisionReactionSystem will not work.");
        return;
    }

    auto state = playerState->GetStateEnum();
    if (state != PlayerMoveState::FALL_DOWN && state != PlayerMoveState::JUMP) {
        return;
    }

    SphereCollider* collider            = GetComponent<SphereCollider>(_handle);
    CollisionPushBackInfo* pushBackInfo = GetComponent<CollisionPushBackInfo>(_handle);

    if (!collider || !pushBackInfo) {
        return;
    }

    float radiusDiff       = collider->GetWorldRadius() - playerCollider->GetWorldRadius();
    float penetrationDepth = 0.f;
    Vec3f collNormal       = Vec3f();
    bool isWheelie         = false;
    for (const auto& [handle, info] : pushBackInfo->GetCollisionInfoMap()) {
        if (info.pushBackType == CollisionPushBackType::None || PlayerMoveUtils::IsHitGround(info.collFaceNormal)) {
            continue;
        }
        float dotVN = playerRigidbody->GetVelocity().normalize().dot(info.collFaceNormal);

        // どれくらい平行に動いているか (1.0 = 完全に平行, 0.0 = 完全に垂直)
        float parallelFactor                       = 1.f - std::fabs(dotVN);
        PlayerMoveUtils::WallContactResult contact = PlayerMoveUtils::EvaluateWallContact(parallelFactor, playerRigidbody, playerStatus);
        if (contact == PlayerMoveUtils::WallContactResult::WallHit) {
            continue;
        }

        // 衝突の深さ
        // 押し戻し量を深度を深度として扱う
        if (penetrationDepth < info.collVec.length()) {
            collNormal       = info.collFaceNormal.normalize();
            penetrationDepth = info.collVec.length();
            isWheelie        = (contact == PlayerMoveUtils::WallContactResult::Wheelie);
        }
    }

    if (penetrationDepth >= OriGine::kEpsilon) {
        float t = std::clamp(penetrationDepth / radiusDiff, 0.f, 1.f);
        // wheelieの時の処理
        if (isWheelie) {
            Vec3f wheelieDir = PlayerMoveUtils::ComputeWheelieDirection(collNormal, axisY);

            // 回転を適用
            Quaternion targetRotation;

            if (wheelieDir.lengthSq() > kEpsilon) {
                // 正面方向(_forward) = climbDirection (壁に沿って空を目指す方向)
                // 上方向(up)       = wallNormal     (壁から垂直に出る方向＝バイクの頭上)
                targetRotation = Quaternion::LookAt(wheelieDir, collNormal);
            } else {
                // 床や天井にいる場合（法線が(0,1,0)に近い）
                Vec3f currentForward = playerRigidbody->GetVelocity().normalize();
                // 現在の進行方向を壁（床）に沿わせる
                float fDot           = currentForward.dot(collNormal);
                Vec3f surfaceForward = currentForward - (collNormal * fDot);

                targetRotation = Quaternion::LookAt(surfaceForward, collNormal);
            }

            targetRotation = Slerp(playerTransform->rotate, playerTransform->rotate * targetRotation, t);
        } else {
            // wallRunの時の処理
            float rotateZOffsetOnWallRun = effectControlParam->GetRotateOffsetOnWallRun();

            rotateZOffsetOnWallRun = std::lerp(0.f, rotateZOffsetOnWallRun, EaseOutCubic(t));

            Vec3f wallFrontDir = PlayerMoveUtils::ComputeWallRunDirection(playerRigidbody->GetVelocity(), collNormal);
            bool isRightWall   = PlayerMoveUtils::IsWallRight(wallFrontDir, collNormal);

            Quaternion angleOffset = Quaternion::RotateAxisAngle(axisZ, isRightWall ? rotateZOffsetOnWallRun : -rotateZOffsetOnWallRun);
            playerTransform->rotate *= angleOffset;
        }
    }
}
