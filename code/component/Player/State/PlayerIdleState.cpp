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

void PlayerIdleState::Initialize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto playerStatus  = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntity);
    auto rigidbody     = scene_->GetComponent<Rigidbody>(playerEntity);
    auto playerInput   = scene_->GetComponent<PlayerInput>(playerEntity);

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

    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);

    // 減速
    rigidbody->SetVelocity(LerpByDeltaTime(rigidbody->GetVelocity(), Vec3f(), _deltaTime, kDecelerationRate));

    // 落下時間を更新
    auto* state = scene_->GetComponent<PlayerState>(playerEntity);
    if (state->IsOnGround()) {
        fallDownTimer_ = kFallDownThresholdTime_;
    } else {
        fallDownTimer_ -= _deltaTime;
    }

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->GetComponent<CameraController>(scene_->GetUniqueEntity("GameCamera"));
    if (cameraController) {
        // カメラのオフセットを徐々に元に戻す
        cameraOffsetLerpTimer_ += _deltaTime;
        float t = cameraOffsetLerpTimer_ / kCameraOffsetLerpTime_;
        t       = std::clamp(t, 0.f, 1.f);

        const Vec3f& targetOffset  = cameraController->GetFirstOffset();
        const Vec3f& currentOffset = cameraController->GetCurrentOffset();
        Vec3f newOffset            = Lerp<3, float>(currentOffset, targetOffset, EaseOutCubic(t));
        cameraController->SetCurrentOffset(newOffset);

        const Vec3f& firstTargetOffset   = cameraController->GetFirstTargetOffset();
        const Vec3f& currentTargetOffset = cameraController->GetCurrentTargetOffset();
        Vec3f newTargetOffset            = Lerp<3, float>(currentTargetOffset, firstTargetOffset, EaseOutCubic(t));
        cameraController->SetCurrentTargetOffset(newTargetOffset);
    }
}

void PlayerIdleState::Finalize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto playerStatus  = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);

    playerStatus->SetCurrentMaxSpeed(playerStatus->GetBaseSpeed());
    rigidbody->SetMaxXZSpeed(playerStatus->GetBaseSpeed());
    // Jump がおかしくなるため しっかりと ゼロ にする
    rigidbody->SetVelocity({0.0f, 0.0f, 0.0f});
}

PlayerMoveState PlayerIdleState::TransitionState() const {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto state         = scene_->GetComponent<PlayerState>(playerEntity);
    auto playerInput   = scene_->GetComponent<PlayerInput>(playerEntity);

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
