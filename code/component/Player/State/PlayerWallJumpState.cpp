#include "PlayerWallJumpState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/camera/CameraController.h"
#include "component/physics/Rigidbody.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"

/// log
#include "logger/Logger.h"
/// math
#include "MyEasing.h"

using namespace OriGine;

void PlayerWallJumpState::Initialize() {
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* playerState  = scene_->GetComponent<PlayerState>(playerEntityHandle_);

    rigidbody->SetAcceleration({0.0f, 0.0f, 0.0f});
    rigidbody->SetUseGravity(false);

    // 壁情報
    const OriGine::Vec3f& wallNormal = playerState->GetWallCollisionNormal().normalize();
    OriGine::Vec3f jumpDirWorld      = Vec3f();
    float jumpSpeed                  = 0.f;

    // プレイヤーの進行方向（正面）
    OriGine::Vec3f velocityDirection = rigidbody->GetVelocity();
    velocityDirection                = velocityDirection.normalize();

    if (playerState->GetPreStateEnum() == PlayerMoveState::WALL_RUN) {
        const OriGine::Vec3f& wallJumpDirection = playerStatus->GetWallJumpOffset();
        // --- 壁ローカル → ワールド変換 ---
        // wallJumpDirection = (x:外, y:上, z:沿う)
        jumpDirWorld =
            wallNormal * wallJumpDirection[X] + axisY * wallJumpDirection[Y] + velocityDirection * wallJumpDirection[Z];
        // wallRun後は maxXZSpeedを使用する
        jumpSpeed = rigidbody->GetMaxXZSpeed() * playerStatus->GetWallRunRate();
    } else {
        const OriGine::Vec3f& wallJumpDirection = playerStatus->GetWheelieJumpOffset();
        // --- 壁ローカル → ワールド変換 ---
        jumpDirWorld =
            velocityDirection * wallJumpDirection[Y] + wallNormal * wallJumpDirection[Z];
        // whellie後は velocityをそのまま使用する
        jumpSpeed = rigidbody->GetVelocity().length();
    }
    jumpDirWorld = jumpDirWorld.normalize();

    // --- 最終速度設定 ---
    velo_ = jumpDirWorld * jumpSpeed;

    rigidbody->SetVelocity(velo_);

    forceJumpTimer_ = 0.f;
}

void PlayerWallJumpState::Update(float _deltaTime) {
    forceJumpTimer_ += _deltaTime;
}

void PlayerWallJumpState::Finalize() {
    auto* rigidbody = scene_->GetComponent<Rigidbody>(playerEntityHandle_);

    rigidbody->SetUseGravity(true); // 重力を有効

    forceJumpTimer_ = kForceJumpTime_; // 強制時間を終了させる
}

PlayerMoveState PlayerWallJumpState::TransitionState() const {
    auto* playerState = scene_->GetComponent<PlayerState>(playerEntityHandle_);

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
