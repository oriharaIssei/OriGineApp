#include "PlayerFallDownState.h"

/// component
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
    Vec3f worldInputDir              = playerInput->CalculateWorldInputDirection(cameraTransform->rotate);
    Vec3f forwardDirection           = playerStatus->ComputeSmoothedDirection(worldInputDir, rigidbody, transform, _deltaTime);
    transform->rotate                = Quaternion::LookAt(forwardDirection, axisY);

    Vec3f newVelo                    = playerStatus->GetCurrentMaxSpeed() * forwardDirection;
    newVelo[Y] = rigidbody->GetVelocity(Y);
    rigidbody->SetVelocity(newVelo);

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->GetComponent<CameraController>(playerState->GetCameraEntityHandle());

    if (!cameraController) {
        return;
    }
    // カメラのオフセットを徐々に元に戻す

    float cameraDeltaTime = Engine::GetInstance()->GetDeltaTimer()->GetScaledDeltaTime("Camera");
    cameraOffsetLerpTimer_ += cameraDeltaTime;
    float t = cameraOffsetLerpTimer_ / kCameraOffsetLerpTime_;
    t       = std::clamp(t, 0.f, 1.f);

    OriGine::Vec3f targetOffset;
    OriGine::Vec3f targetTargetOffset;
    if (playerState->GetGearLevel() >= kThresholdGearLevelOfCameraOffset_) {
        targetOffset       = cameraController->offsetOnDash;
        targetTargetOffset = cameraController->targetOffsetOnDash;
    } else {
        targetOffset       = cameraController->firstOffset;
        targetTargetOffset = cameraController->firstTargetOffset;
    }

    cameraController->currentOffset       = Lerp<3, float>(cameraController->currentOffset, targetOffset, EaseOutCubic(t));
    cameraController->currentTargetOffset = Lerp<3, float>(cameraController->currentTargetOffset, targetTargetOffset, EaseOutCubic(t));
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
}

PlayerMoveState PlayerFallDownState::TransitionState() const {
    auto state       = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto playerInput = scene_->GetComponent<PlayerInput>(playerEntityHandle_);

    // Rail上にいる場合
    if (state->IsOnRail()) {
        return PlayerMoveState::RUN_ON_RAIL;
    }
    // 壁走り判定
    if (state->IsCollisionWithWall()) {
        if (state->IsWheelie()) {
            return PlayerMoveState::WHEELIE_RUN;
        }
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
