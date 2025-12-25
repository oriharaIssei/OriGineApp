#include "PlayerFallDownState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/physics/Rigidbody.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

#include "component/Camera/CameraController.h"

/// math
#include "MyEasing.h"

using namespace OriGine;

void PlayerFallDownState::Initialize() {}

void PlayerFallDownState::Update(float _deltaTime) {
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* playerState  = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* playerInput  = scene_->GetComponent<PlayerInput>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);

    // 速度更新
    CameraTransform* cameraTransform = scene_->GetComponent<CameraTransform>(playerState->GetCameraEntityHandle());
    playerStatus->UpdateAccel(_deltaTime, playerInput, transform, rigidbody, cameraTransform->rotate);

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->GetComponent<CameraController>(playerState->GetCameraEntityHandle());

    if (!cameraController) {
        return;
    }
    // カメラのオフセットを徐々に元に戻す
    cameraOffsetLerpTimer_ += _deltaTime;
    float t = cameraOffsetLerpTimer_ / kCameraOffsetLerpTime_;
    t       = std::clamp(t, 0.f, 1.f);

    OriGine::Vec3f targetOffset, currentOffset;
    OriGine::Vec3f targetTargetOffset, currentTargetOffset;
    if (playerState->GetGearLevel() >= kThresholdGearLevelOfCameraOffset_) {
        targetOffset  = cameraController->GetOffsetOnDash();
        currentOffset = cameraController->GetCurrentOffset();

        targetTargetOffset  = cameraController->GetTargetOffsetOnDash();
        currentTargetOffset = cameraController->GetCurrentTargetOffset();
    } else {
        targetOffset  = cameraController->GetFirstOffset();
        currentOffset = cameraController->GetCurrentOffset();

        targetTargetOffset  = cameraController->GetFirstTargetOffset();
        currentTargetOffset = cameraController->GetCurrentTargetOffset();
    }

    OriGine::Vec3f newTargetOffset = Lerp<3, float>(currentTargetOffset, targetTargetOffset, EaseOutCubic(t));
    OriGine::Vec3f newOffset       = Lerp<3, float>(currentOffset, targetOffset, EaseOutCubic(t));
    cameraController->SetCurrentOffset(newOffset);
    cameraController->SetCurrentTargetOffset(newTargetOffset);
}

void PlayerFallDownState::Finalize() {
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);

    rigidbody->SetAcceleration({0.0f, 0.0f, 0.0f}); // 加速度をリセット
    OriGine::Vec3f velo = rigidbody->GetVelocity();

    float limitSpeed = playerStatus->GetCurrentMaxSpeed();
    if (velo.lengthSq() >= limitSpeed * limitSpeed) {
        // 速度が速すぎる場合は制限
        velo = velo.normalize() * limitSpeed;
        rigidbody->SetVelocity(velo);
    }

    auto* skinningAnim = scene_->GetComponent<SkinningAnimationComponent>(playerEntityHandle_);
    if (skinningAnim) {
        skinningAnim->Play(4); // 差し替え
    }
}

PlayerMoveState PlayerFallDownState::TransitionState() const {
    auto state         = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto playerInput   = scene_->GetComponent<PlayerInput>(playerEntityHandle_);

    // 壁走り判定
    if (state->IsCollisionWithWall()) {
        return PlayerMoveState::WALL_RUN;
    }
    // 着地判定
    if (state->IsOnGround()) {
        if (playerInput->GetInputDirection().lengthSq() > 0.f) {
            return PlayerMoveState::DASH;
        }
        return PlayerMoveState::IDLE;
    }

    return PlayerMoveState::FALL_DOWN;
}
