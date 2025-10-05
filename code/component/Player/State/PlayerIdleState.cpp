#include "PlayerIdleState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/effect/particle/emitter/Emitter.h"
#include "component/physics/Rigidbody.h"

#include "component/Camera/CameraController.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

/// math
#include "MyEasing.h"

void PlayerIdleState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto playerStatus  = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* state        = scene_->getComponent<PlayerState>(playerEntity);
    auto rigidbody     = scene_->getComponent<Rigidbody>(playerEntity);
    rigidbody->setAcceleration({0.f, 0.0f, 0.0f});
    playerStatus->setCurrentMaxSpeed(0.0f);
    playerStatus->setGearUpCoolTime(playerStatus->getBaseGearupCoolTime());
    state->setGearLevel(kDefaultPlayerGearLevel);

    cameraOffsetLerpTimer_ = 0.0f;
}

void PlayerIdleState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    // 減速
    rigidbody->setVelocity(rigidbody->getVelocity() - (rigidbody->getVelocity() * 0.76f));

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->getComponent<CameraController>(scene_->getUniqueEntity("GameCamera"));
    if (cameraController) {
        // カメラのオフセットを徐々に元に戻す
        cameraOffsetLerpTimer_ += _deltaTime;
        float t = cameraOffsetLerpTimer_ / kCameraOffsetLerpTime_;
        t       = std::clamp(t, 0.f, 1.f);

        const Vec3f& targetOffset  = cameraController->getFirstOffset();
        const Vec3f& currentOffset = cameraController->getCurrentOffset();
        Vec3f newOffset            = Lerp<3, float>(currentOffset, targetOffset, EaseOutCubic(t));
        cameraController->setCurrentOffset(newOffset);

        const Vec3f& firstTargetOffset   = cameraController->getFirstTargetOffset();
        const Vec3f& currentTargetOffset = cameraController->getCurrentTargetOffset();
        Vec3f newTargetOffset            = Lerp<3, float>(currentTargetOffset, firstTargetOffset, EaseOutCubic(t));
        cameraController->setCurrentTargetOffset(newTargetOffset);
    }
}

void PlayerIdleState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto playerStatus  = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    playerStatus->setCurrentMaxSpeed(playerStatus->getBaseSpeed());
    rigidbody->setMaxXZSpeed(playerStatus->getBaseSpeed());
    // Jump がおかしくなるため しっかりと ゼロ にする
    rigidbody->setVelocity({0.0f, 0.0f, 0.0f});
}

PlayerMoveState PlayerIdleState::TransitionState() const {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto state         = scene_->getComponent<PlayerState>(playerEntity);
    auto playerInput   = scene_->getComponent<PlayerInput>(playerEntity);

    if (state->isOnGround()) {
        if (playerInput->isJumpInput()) {
            return PlayerMoveState::JUMP;
        }
        if (playerInput->getInputDirection().lengthSq() > 0.f) {
            return PlayerMoveState::DASH;
        }

        return PlayerMoveState::IDLE;
    } else {
        // 空中にいる場合はジャンプ状態に遷移
        return PlayerMoveState::FALL_DOWN;
    }
}
