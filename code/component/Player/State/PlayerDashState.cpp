#include "PlayerDashState.h"

/// engine
#include "Engine.h"

/// component
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/Camera/CameraController.h"
#include "component/physics/Rigidbody.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

/// math
#include "MyEasing.h"
#include <mathEnv.h>

using namespace OriGine;

void PlayerDashState::Initialize() {
    auto* state        = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);

    // 速度を初期化
    int32_t gearLevel  = state->GetGearLevel();
    float currentMaxSpeed = playerStatus->CalculateSpeedByGearLevel(gearLevel);
    playerStatus->SetCurrentMaxSpeed(currentMaxSpeed);
    // プレイヤーの向いている方向に速度を設定
    Vec3f forwardDir = transform->CalculateWorldRotate().RotateVector(axisZ);
    forwardDir[Y]    = 0.f;
    forwardDir       = forwardDir.normalize();
    rigidbody->SetVelocity(forwardDir * currentMaxSpeed);

    // 落下タイマーをリセット
    cameraOffsetLerpTimer_ = 0.f;
}

void PlayerDashState::Update(float _deltaTime) {
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* playerInput  = scene_->GetComponent<PlayerInput>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);

    /// ---------------------------------------------------------------------
    // gearLevel の更新 (走っている間だけ)
    playerStatus->minusGearUpCoolTime(_deltaTime);

    int32_t gearLevel = state->GetGearLevel();
    // ギアレベルが最大に達していない場合、
    if (gearLevel < kMaxPlayerGearLevel - 1) {
        // クールタイムが0以下になったらギアレベルを上げる
        if (playerStatus->GetGearUpCoolTime() <= 0.f) {
            auto& stateFlag = state->GetStateFlagRef();
            stateFlag.SetCurrent(stateFlag.Current() | PlayerStateFlag::GEAR_UP);

            ++gearLevel;
            state->SetGearLevel(gearLevel);

            playerStatus->SetGearUpCoolTime(playerStatus->CalculateCoolTimeByGearLevel(gearLevel));

            playerStatus->SetCurrentMaxSpeed(playerStatus->CalculateSpeedByGearLevel(gearLevel));
        }
    }

    // 速度更新
    CameraTransform* cameraTransform = scene_->GetComponent<CameraTransform>(state->GetCameraEntityHandle());
    Vec3f worldInputDir              = playerInput->CalculateWorldInputDirection(cameraTransform->rotate);
    Vec3f forwardDirection           = playerStatus->ComputeSmoothedDirection(worldInputDir, rigidbody, transform, _deltaTime);
    transform->rotate                = Quaternion::LookAt(forwardDirection, axisY);
    playerStatus->UpdateAccel(_deltaTime, forwardDirection, rigidbody);

    // 落下時間を更新
    if (state->IsOnGround()) {
        fallDownTimer_ = kFallDownThresholdTime_;
    } else {
        fallDownTimer_ -= _deltaTime;
    }

    if (kThresholdGearLevelOfCameraOffset_ >= gearLevel) {
        return;
    }

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->GetComponent<CameraController>(state->GetCameraEntityHandle());

    if (cameraController) {
        float cameraDeltaTime = Engine::GetInstance()->GetDeltaTimer()->GetScaledDeltaTime("Camera");
        // カメラのオフセットを徐々に元に戻す
        cameraOffsetLerpTimer_ += cameraDeltaTime;
        float t = cameraOffsetLerpTimer_ / kCameraOffsetLerpTime_;
        t       = std::clamp(t, 0.f, 1.f);

        cameraController->currentOffset       = Lerp<3, float>(cameraController->currentOffset, cameraController->offsetOnDash, EaseOutCubic(t));
        cameraController->currentTargetOffset = Lerp<3, float>(cameraController->currentTargetOffset, cameraController->targetOffsetOnDash, EaseOutCubic(t));
    }
}

void PlayerDashState::Finalize() {
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);

    OriGine::Vec3f velo = rigidbody->GetVelocity();

    float limitSpeed = playerStatus->GetCurrentMaxSpeed(); // このフレームに追加される速度を引く
    if (velo.lengthSq() >= limitSpeed * limitSpeed) {
        // 速度が速すぎる場合は制限
        velo = velo.normalize() * limitSpeed;
        rigidbody->SetVelocity(velo);
    }
}

PlayerMoveState PlayerDashState::TransitionState() const {
    auto state       = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto playerInput = scene_->GetComponent<PlayerInput>(playerEntityHandle_);
    auto rigidbody   = scene_->GetComponent<Rigidbody>(playerEntityHandle_);

    // 入力がない場合はアイドル状態に遷移
    if (playerInput->GetInputDirection().lengthSq() <= 0.f) {
        return PlayerMoveState::IDLE;
    }

    // 上昇速度がある場合は落下状態に遷移
    if (rigidbody->GetVelocity()[Y] > kEpsilon) {
        return PlayerMoveState::FALL_DOWN;
    }

    // Rail上にいる場合
    if (state->IsOnRail()) {
        return PlayerMoveState::RUN_ON_RAIL;
    }
    // 地面にいる場合
    if (state->IsOnGround()) {
        if (playerInput->IsJumpInput()) {
            return PlayerMoveState::JUMP;
        }
    } else {
        // 空中にいる場合は落下状態に遷移
        // 一定時間経過したら落下状態に遷移
        if (fallDownTimer_ <= 0.f) {
            return PlayerMoveState::FALL_DOWN;
        }
    }

    return PlayerMoveState::DASH;
}
