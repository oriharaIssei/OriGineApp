#include "PlayerWallJumpState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/Camera/CameraController.h"
#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

/// log
#include "logger/Logger.h"
/// math
#include "MyEasing.h"

void PlayerWallJumpState::Initialize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* playerState  = scene_->GetComponent<PlayerState>(playerEntity);

    rigidbody->SetAcceleration({0.0f, 0.0f, 0.0f});
    rigidbody->SetUseGravity(false);

    // 壁情報
    const Vec3f& wallNormal        = playerState->GetWallCollisionNormal().normalize();
    const Vec3f& wallJumpDirection = playerStatus->GetWallJumpOffset();

    // プレイヤーの進行方向（正面）
    Vec3f velocityDirection = rigidbody->GetVelocity();
    velocityDirection       = velocityDirection.normalize();

    // --- 壁ローカル → ワールド変換 ---
    // wallJumpDirection = (x:外, y:上, z:沿う)
    Vec3f jumpDirWorld =
        wallNormal * wallJumpDirection[X] + axisY * wallJumpDirection[Y] + velocityDirection * wallJumpDirection[Z];

    jumpDirWorld = jumpDirWorld.normalize();

    // --- 最終速度設定 ---
    float jumpSpeed = rigidbody->GetMaxXZSpeed() * playerStatus->GetWallRunRate();
    velo_           = jumpDirWorld * jumpSpeed;

    rigidbody->SetVelocity(velo_);

    forceJumpTimer_ = 0.f;
}

void PlayerWallJumpState::Update(float _deltaTime) {
    forceJumpTimer_ += _deltaTime;
}

void PlayerWallJumpState::Finalize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);

    rigidbody->SetUseGravity(true); // 重力を有効

    forceJumpTimer_ = kForceJumpTime_; // 強制時間を終了させる
}

PlayerMoveState PlayerWallJumpState::TransitionState() const {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);

    auto* playerState = scene_->GetComponent<PlayerState>(playerEntity);

    if (playerState->IsOnGround()) {
        return PlayerMoveState::DASH;
    }
    if (playerState->IsCollisionWithWall()) {
        return PlayerMoveState::WALL_RUN;
    }

    // 強制時間が終了したら 落下状態へ
    if (forceJumpTimer_ >= kForceJumpTime_) {
        return PlayerMoveState::FALL_DOWN;
    }

    return PlayerMoveState::WALL_JUMP;
}
