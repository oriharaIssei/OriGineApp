#include "PlayerFallDownState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

#include "component/Camera/CameraController.h"

/// math
#include "MyEasing.h"

void PlayerFallDownState::Initialize() {}

void PlayerFallDownState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* playerState  = scene_->getComponent<PlayerState>(playerEntity);
    auto* playerInput  = scene_->getComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);

    // 速度更新
    playerStatus->UpdateAccel(playerInput, transform, rigidbody, scene_->getComponent<CameraTransform>(scene_->getUniqueEntity("GameCamera"))->rotate);

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->getComponent<CameraController>(scene_->getUniqueEntity("GameCamera"));

    if (cameraController) {

        // カメラのオフセットを徐々に元に戻す
        cameraOffsetLerpTimer_ += _deltaTime;
        float t = cameraOffsetLerpTimer_ / kCameraOffsetLerpTime_;
        t       = std::clamp(t, 0.f, 1.f);

        Vec3f targetOffset, currentOffset;
        Vec3f targetTargetOffset, currentTargetOffset;
        if (playerState->getGearLevel() >= kThresholdGearLevelOfCameraOffset_) {
            targetOffset  = cameraController->getOffsetOnDash();
            currentOffset = cameraController->getCurrentOffset();

            targetTargetOffset  = cameraController->getTargetOffsetOnDash();
            currentTargetOffset = cameraController->getCurrentTargetOffset();
        } else {
            targetOffset  = cameraController->getFirstOffset();
            currentOffset = cameraController->getCurrentOffset();

            targetTargetOffset  = cameraController->getFirstTargetOffset();
            currentTargetOffset = cameraController->getCurrentTargetOffset();
        }

        Vec3f newTargetOffset = Lerp<3, float>(currentTargetOffset, targetTargetOffset, EaseOutCubic(t));
        Vec3f newOffset       = Lerp<3, float>(currentOffset, targetOffset, EaseOutCubic(t));
        cameraController->setCurrentOffset(newOffset);
        cameraController->setCurrentTargetOffset(newTargetOffset);
    }
}

void PlayerFallDownState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f}); // 加速度をリセット
    Vec3f velo = rigidbody->getVelocity();

    float limitSpeed = playerStatus->getCurrentMaxSpeed();
    if (velo.lengthSq() >= limitSpeed * limitSpeed) {
        // 速度が速すぎる場合は制限
        velo = velo.normalize() * limitSpeed;
        rigidbody->setVelocity(velo);
    }

    auto* skinningAnim = scene_->getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->Play(4); // 差し替え
    }
}

PlayerMoveState PlayerFallDownState::TransitionState() const {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto state         = scene_->getComponent<PlayerState>(playerEntity);
    auto playerInput   = scene_->getComponent<PlayerInput>(playerEntity);

    // 壁走り判定
    if (state->isCollisionWithWall()) {
        return PlayerMoveState::WALL_RUN;
    }
    // 着地判定
    if (state->isOnGround()) {
        if (playerInput->getInputDirection().lengthSq() > 0.f) {
            return PlayerMoveState::DASH;
        }
        return PlayerMoveState::IDLE;
    }

    return PlayerMoveState::FALL_DOWN;
}
