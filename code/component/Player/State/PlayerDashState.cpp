#include "PlayerDashState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/Camera/CameraController.h"
#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

/// math
#include "MyEasing.h"

void PlayerDashState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* state        = scene_->getComponent<PlayerState>(playerEntity);
    auto* status       = scene_->getComponent<PlayerStatus>(playerEntity);

    auto* skinningAnim = scene_->getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->setPlaybackSpeed(1, 1.f + float(state->getGearLevel()));
    }

    auto* rigidbody = scene_->getComponent<Rigidbody>(playerEntity);
    rigidbody->setMaxXZSpeed(status->getCurrentMaxSpeed());

    cameraOffsetLerpTimer_ = 0.f;
}

void PlayerDashState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* state        = scene_->getComponent<PlayerState>(playerEntity);
    auto* playerInput  = scene_->getComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);

    /// ---------------------------------------------------------------------
    // gearLevel の更新
    playerStatus->minusGearUpCoolTime(_deltaTime);

    if (state->getGearLevel() < kMaxPlayerGearLevel) {
        if (playerStatus->getGearUpCoolTime() <= 0.f) {
            state->setGearUp(true);

            int32_t addedGearLevel = state->getGearLevel() + 1;
            state->setGearLevel(addedGearLevel);

            playerStatus->setGearUpCoolTime(playerStatus->CalculateCoolTimeByGearLevel(addedGearLevel));

            playerStatus->setCurrentMaxSpeed(playerStatus->CalculateSpeedByGearLevel(addedGearLevel));
            rigidbody->setMaxXZSpeed(playerStatus->getCurrentMaxSpeed());

            auto* skinningAnim = scene_->getComponent<SkinningAnimationComponent>(playerEntity);
            if (skinningAnim) {
                skinningAnim->setPlaybackSpeed(1, 1.f + float(state->getGearLevel()));
            }
        }
    }

    playerStatus->UpdateAccel(playerInput, transform, rigidbody, scene_->getComponent<CameraTransform>(scene_->getUniqueEntity("GameCamera"))->rotate);

    if (kThresholdGearLevelOfCameraOffset_ >= state->getGearLevel()) {
        return;
    }

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->getComponent<CameraController>(scene_->getUniqueEntity("GameCamera"));

    if (cameraController) {
        // カメラのオフセットを徐々に元に戻す
        cameraOffsetLerpTimer_ += _deltaTime;
        float t = cameraOffsetLerpTimer_ / kCameraOffsetLerpTime_;
        t       = std::clamp(t, 0.f, 1.f);

        const Vec3f& targetOffset  = cameraController->getOffsetOnDash();
        const Vec3f& currentOffset = cameraController->getCurrentOffset();
        Vec3f newOffset            = Lerp<3, float>(currentOffset, targetOffset, EaseOutCubic(t));
        cameraController->setCurrentOffset(newOffset);

        const Vec3f& targetTargetOffset  = cameraController->getTargetOffsetOnDash();
        const Vec3f& currentTargetOffset = cameraController->getCurrentTargetOffset();
        Vec3f newTargetOffset            = Lerp<3, float>(currentTargetOffset, targetTargetOffset, EaseOutCubic(t));
        cameraController->setCurrentTargetOffset(newTargetOffset);
    }
}

void PlayerDashState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    Vec3f velo = rigidbody->getVelocity();

    float limitSpeed = playerStatus->getCurrentMaxSpeed(); // このフレームに追加される速度を引く
    if (velo.lengthSq() >= limitSpeed * limitSpeed) {
        // 速度が速すぎる場合は制限
        velo = velo.normalize() * limitSpeed;
        rigidbody->setVelocity(velo);
    }
    auto* skinningAnim = scene_->getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->setPlaybackSpeed(1, 1.f);
    }
}

PlayerMoveState PlayerDashState::TransitionState() const {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto state         = scene_->getComponent<PlayerState>(playerEntity);
    auto playerInput   = scene_->getComponent<PlayerInput>(playerEntity);

    if (playerInput->getInputDirection().lengthSq() <= 0.f) {
        return PlayerMoveState::IDLE;
    }

    if (state->isOnGround()) {
        if (playerInput->isJumpInput()) {
            return PlayerMoveState::JUMP;
        }
    } else {
        // 空中にいる場合はジャンプ状態に遷移
        return PlayerMoveState::FALL_DOWN;
    }

    return PlayerMoveState::DASH;
}
