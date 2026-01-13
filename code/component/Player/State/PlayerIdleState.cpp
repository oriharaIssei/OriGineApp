#include "PlayerIdleState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/effect/particle/emitter/Emitter.h"
#include "component/physics/Rigidbody.h"

#include "component/Camera/CameraController.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

/// math
#include "math/Interpolation.h"
#include "MyEasing.h"

using namespace OriGine;

void PlayerIdleState::Initialize() {
    auto playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* state       = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto playerInput  = scene_->GetComponent<PlayerInput>(playerEntityHandle_);

    /// 初期化処理
    // 入力方向をリセット
    playerInput->SetWorldInputDirection({0.f, 0.f, 0.f});
    // 速度をゼロにする
    rigidbody->SetAcceleration({0.f, 0.0f, 0.0f});
    playerStatus->SetCurrentMaxSpeed(0.0f);
    // ギアアップのクールタイム&ギアレベルをリセット
    playerStatus->SetGearUpCoolTime(playerStatus->GetBaseGearupCoolTime());
    state->SetGearLevel(kDefaultPlayerGearLevel);

    cameraOffsetLerpTimer_ = 0.0f;
}

void PlayerIdleState::Update(float _deltaTime) {
    constexpr float kDecelerationRate = 1.8f;

    auto* rigidbody = scene_->GetComponent<Rigidbody>(playerEntityHandle_);

    // 減速
    rigidbody->SetVelocity(LerpByDeltaTime(rigidbody->GetVelocity(), OriGine::Vec3f(), _deltaTime, kDecelerationRate));

    // 落下時間を更新
    auto* state = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    if (state->IsOnGround()) {
        fallDownTimer_ = kFallDownThresholdTime_;
    } else {
        fallDownTimer_ -= _deltaTime;
    }

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->GetComponent<CameraController>(state->GetCameraEntityHandle());
    if (cameraController) {
        // カメラのオフセットを徐々に元に戻す
        cameraOffsetLerpTimer_ += _deltaTime;
        float t = cameraOffsetLerpTimer_ / kCameraOffsetLerpTime_;
        t       = std::clamp(t, 0.f, 1.f);

        cameraController->currentOffset = Lerp<3, float>(cameraController->currentOffset, cameraController->firstOffset, EaseOutCubic(t));
        cameraController->currentTargetOffset = Lerp<3, float>(cameraController->currentTargetOffset, cameraController->firstTargetOffset, EaseOutCubic(t));
    }
}

void PlayerIdleState::Finalize() {
    auto playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* rigidbody   = scene_->GetComponent<Rigidbody>(playerEntityHandle_);

    playerStatus->SetCurrentMaxSpeed(playerStatus->GetBaseSpeed());
    rigidbody->SetMaxXZSpeed(playerStatus->GetBaseSpeed());
    // Jump がおかしくなるため しっかりと ゼロ にする
    rigidbody->SetVelocity({0.0f, 0.0f, 0.0f});
}

PlayerMoveState PlayerIdleState::TransitionState() const {
    auto state       = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto playerInput = scene_->GetComponent<PlayerInput>(playerEntityHandle_);

    if (state->IsOnGround()) {
        if (playerInput->IsJumpInput()) {
            return PlayerMoveState::JUMP;
        }
        if (playerInput->GetInputDirection().lengthSq() > 0.f) {
            return PlayerMoveState::DASH;
        }

        return PlayerMoveState::IDLE;
    } else {
        if (fallDownTimer_ <= 0.f) {
            return PlayerMoveState::FALL_DOWN;
        }
    }
    return PlayerMoveState::IDLE;
}
