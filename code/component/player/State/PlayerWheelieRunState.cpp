#include "PlayerWheelieRunState.h"

/// ECS
// component
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/State/PlayerState.h"

/// math
#include "math/mathEnv.h"

using namespace OriGine;

void PlayerWheelieRunState::Initialize() {
    constexpr float kUpwardSpeedCorrectionValue = 0.4f;

    auto* playerState = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* rigidbody   = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* transform   = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);

    // 投影
    wallNormal_          = playerState->GetWallCollisionNormal().normalize();
    float dot            = Vec3f::Dot(wallNormal_, axisY);
    Vec3f climbDirection = axisY - (wallNormal_ * dot);
    climbDirection       = climbDirection.normalize();

    // 回転を適用
    Quaternion targetRotation;

    // climbDirectionの長さがほぼ0なら、床か天井にいる（＝登れない）
    if (climbDirection.lengthSq() > kEpsilon) {
        // 【ここがポイント】
        // 正面方向(_forward) = climbDirection (壁に沿って空を目指す方向)
        // 上方向(up)       = wallNormal     (壁から垂直に出る方向＝バイクの頭上)
        targetRotation = Quaternion::LookAt(climbDirection, wallNormal_);
    } else {
        // 床や天井にいる場合（法線が(0,1,0)に近い）は、
        // 「登る」概念がないので、現在の進行方向を維持する等の処理
        Vec3f currentForward = rigidbody->GetVelocity().normalize();
        // 現在の進行方向を壁（床）に沿わせる
        float fDot           = currentForward.dot(wallNormal_);
        Vec3f surfaceForward = currentForward - (wallNormal_ * fDot);

        targetRotation = Quaternion::LookAt(surfaceForward, wallNormal_);
    }

    transform->rotate = targetRotation;

    // velocityの方向を進行方向に合わせる
    const Vec3f& currentVelo = rigidbody->GetVelocity();
    float newSpeed           = Vec2f(currentVelo[X], currentVelo[Z]).length();
    newSpeed += currentVelo[Y] * kUpwardSpeedCorrectionValue;
    rigidbody->SetVelocity(newSpeed * climbDirection);

    // 壁との分離猶予
    separationLeftTime_ = separationGraceTime_;
}
void PlayerWheelieRunState::Finalize() {
    auto* transform = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);
    transform->translate += wallNormal_ * 0.1f;
}

void PlayerWheelieRunState::Update(float _deltaTime) {
    auto* transform   = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);
    auto* playerState = scene_->GetComponent<PlayerState>(playerEntityHandle_);

    // 衝突が途切れないようにめり込ませる
    transform->translate -= wallNormal_ * 0.1f;

    // 壁との衝突判定の残り時間を更新
    // これが0以下になると 壁から離れた と判定される
    if (playerState->IsCollisionWithWall() && playerState->IsWheelie()) {
        separationLeftTime_ = separationGraceTime_;
    } else {
        separationLeftTime_ -= _deltaTime;
    }
}

PlayerMoveState PlayerWheelieRunState::TransitionState() const {
    auto* rigidbody = scene_->GetComponent<Rigidbody>(playerEntityHandle_);

    if (separationLeftTime_ <= 0.0f) {
        return PlayerMoveState::FALL_DOWN;
    }

    if (rigidbody->GetVelocity()[Y] <= 0.f) {
        return PlayerMoveState::FALL_DOWN;
    }

    auto playerInput = scene_->GetComponent<PlayerInput>(playerEntityHandle_);
    if (playerInput->IsWallJumpInput()) {
        return PlayerMoveState::WALL_JUMP;
    }

    return PlayerMoveState::WHEELIE_RUN;
}
