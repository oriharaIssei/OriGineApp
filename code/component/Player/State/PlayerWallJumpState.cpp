#include "PlayerWallJumpState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/Camera/CameraController.h"
#include "component/collider/Collider.h"
#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

/// log
#include "logger/Logger.h"
/// math
#include "MyEasing.h"

void PlayerWallJumpState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* playerState  = scene_->getComponent<PlayerState>(playerEntity);

    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f});
    rigidbody->setUseGravity(false);

    // 壁情報
    const Vec3f& wallNormal        = playerState->getWallCollisionNormal().normalize();
    const Vec3f& wallJumpDirection = playerStatus->getWallJumpOffset();

    // プレイヤーの進行方向（正面）
    Vec3f velocityDirection = rigidbody->getVelocity();
    velocityDirection       = velocityDirection.normalize();

    // --- 壁ローカル → ワールド変換 ---
    // wallJumpDirection = (x:外, y:上, z:沿う)
    Vec3f jumpDirWorld =
        wallNormal * wallJumpDirection[X] + axisY * wallJumpDirection[Y] + velocityDirection * wallJumpDirection[Z];

    jumpDirWorld = jumpDirWorld.normalize();

    // --- 最終速度設定 ---
    float jumpSpeed = rigidbody->getMaxXZSpeed() * playerStatus->getWallRunRate();
    velo_           = jumpDirWorld * jumpSpeed;

    rigidbody->setVelocity(velo_);
}

void PlayerWallJumpState::Update(float /*_deltaTime*/) {}

void PlayerWallJumpState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    rigidbody->setUseGravity(true); // 重力を有効
}

PlayerMoveState PlayerWallJumpState::TransitionState() const {

    return PlayerMoveState::FALL_DOWN;
}
