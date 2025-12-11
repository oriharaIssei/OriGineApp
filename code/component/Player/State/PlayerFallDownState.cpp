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
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* playerState  = scene_->GetComponent<PlayerState>(playerEntity);
    auto* playerInput  = scene_->GetComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntity);

    // 速度更新
    OriGine::Entity* cameraEntity = scene_->GetEntity(playerState->GetCameraEntityID());
    playerStatus->UpdateAccel(_deltaTime, playerInput, transform, rigidbody, scene_->GetComponent<CameraTransform>(cameraEntity)->rotate);

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->GetComponent<CameraController>(cameraEntity);

    if (cameraController) {

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
}

void PlayerFallDownState::Finalize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);

    rigidbody->SetAcceleration({0.0f, 0.0f, 0.0f}); // 加速度をリセット
    OriGine::Vec3f velo = rigidbody->GetVelocity();

    float limitSpeed = playerStatus->GetCurrentMaxSpeed();
    if (velo.lengthSq() >= limitSpeed * limitSpeed) {
        // 速度が速すぎる場合は制限
        velo = velo.normalize() * limitSpeed;
        rigidbody->SetVelocity(velo);
    }

    auto* skinningAnim = scene_->GetComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->Play(4); // 差し替え
    }
}

PlayerMoveState PlayerFallDownState::TransitionState() const {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto state         = scene_->GetComponent<PlayerState>(playerEntity);
    auto playerInput   = scene_->GetComponent<PlayerInput>(playerEntity);

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
