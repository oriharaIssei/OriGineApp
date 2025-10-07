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
#include "component/Stage/Stage.h"
#include "component/Stage/StageWall.h"

/// log
#include "logger/Logger.h"
/// math
#include "MyEasing.h"

void PlayerWallJumpState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* playerState  = scene_->getComponent<PlayerState>(playerEntity);

    /// ========================================
    // 速度の初期化
    /// ========================================
    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f}); // 壁ジャンプ時は加速度をリセット
    rigidbody->setUseGravity(false); // 無効

    /// ========================================
    // 目的地を決め, 方向を決める
    /// ========================================
    // 目的のControlPoint への 差分ベクトルを進行方向とする

    Vec3f targetNormal = playerState->getWallCollisionNormal();

    Vec3f wallJumpDirection = playerStatus->getWallJumpOffset() * targetNormal;

    wallJumpDirection = wallJumpDirection.normalize();

    velo_ = wallJumpDirection * (rigidbody->getMaxXZSpeed() * playerStatus->getWallRunRate());
    rigidbody->setVelocity(velo_); // 壁ジャンプの方向に速度を設定
}

void PlayerWallJumpState::Update(float /*_deltaTime*/) {}

void PlayerWallJumpState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    rigidbody->setUseGravity(true); // 重力を有効
}

PlayerMoveState PlayerWallJumpState::TransitionState() const {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto state         = scene_->getComponent<PlayerState>(playerEntity);

    return PlayerMoveState::FALL_DOWN;
}
