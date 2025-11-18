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
#include "component/renderer/MeshRenderer.h"

/// math
#include "MyEasing.h"

void PlayerDashState::Initialize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntity);
    auto* status       = scene_->GetComponent<PlayerStatus>(playerEntity);

    // gearLevel に応じてアニメーション速度と移動速度を設定
    auto* skinningAnim = scene_->GetComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->SetPlaybackSpeed(1, 1.f + float(state->GetGearLevel()));
    }

    // 移動速度の設定
    auto* rigidbody = scene_->GetComponent<Rigidbody>(playerEntity);
    rigidbody->SetMaxXZSpeed(status->GetCurrentMaxSpeed());

    cameraOffsetLerpTimer_ = 0.f;
}

void PlayerDashState::Update(float _deltaTime) {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntity);
    auto* playerInput  = scene_->GetComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->GetComponent<Transform>(playerEntity);

    /// ---------------------------------------------------------------------
    // gearLevel の更新
    playerStatus->minusGearUpCoolTime(_deltaTime);

    int32_t gearLevel = state->GetGearLevel();
    // ギアレベルが最大に達していない場合、
    if (gearLevel < kMaxPlayerGearLevel - 1) {
        // クールタイムが0以下になったらギアレベルを上げる
        if (playerStatus->GetGearUpCoolTime() <= 0.f) {
            state->SetGearUp(true);

            ++gearLevel;
            state->SetGearLevel(gearLevel);

            playerStatus->SetGearUpCoolTime(playerStatus->CalculateCoolTimeByGearLevel(gearLevel));

            playerStatus->SetCurrentMaxSpeed(playerStatus->CalculateSpeedByGearLevel(gearLevel));
            rigidbody->SetMaxXZSpeed(playerStatus->GetCurrentMaxSpeed());
        }
    }

    // 速度更新
    playerStatus->UpdateAccel(_deltaTime, playerInput, transform, rigidbody, scene_->GetComponent<CameraTransform>(scene_->GetUniqueEntity("GameCamera"))->rotate);

    if (kThresholdGearLevelOfCameraOffset_ >= gearLevel) {
        return;
    }

    ///! TODO : ここにカメラの処理を書くべきではない
    CameraController* cameraController = scene_->GetComponent<CameraController>(scene_->GetUniqueEntity("GameCamera"));

    if (cameraController) {
        // カメラのオフセットを徐々に元に戻す
        cameraOffsetLerpTimer_ += _deltaTime;
        float t = cameraOffsetLerpTimer_ / kCameraOffsetLerpTime_;
        t       = std::clamp(t, 0.f, 1.f);

        const Vec3f& targetOffset  = cameraController->GetOffsetOnDash();
        const Vec3f& currentOffset = cameraController->GetCurrentOffset();
        Vec3f newOffset            = Lerp<3, float>(currentOffset, targetOffset, EaseOutCubic(t));
        cameraController->SetCurrentOffset(newOffset);

        const Vec3f& targetTargetOffset  = cameraController->GetTargetOffsetOnDash();
        const Vec3f& currentTargetOffset = cameraController->GetCurrentTargetOffset();
        Vec3f newTargetOffset            = Lerp<3, float>(currentTargetOffset, targetTargetOffset, EaseOutCubic(t));
        cameraController->SetCurrentTargetOffset(newTargetOffset);
    }
}

void PlayerDashState::Finalize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);

    Vec3f velo = rigidbody->GetVelocity();

    float limitSpeed = playerStatus->GetCurrentMaxSpeed(); // このフレームに追加される速度を引く
    if (velo.lengthSq() >= limitSpeed * limitSpeed) {
        // 速度が速すぎる場合は制限
        velo = velo.normalize() * limitSpeed;
        rigidbody->SetVelocity(velo);
    }
    auto* skinningAnim = scene_->GetComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->SetPlaybackSpeed(1, 1.f);
    }
}

PlayerMoveState PlayerDashState::TransitionState() const {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto state         = scene_->GetComponent<PlayerState>(playerEntity);
    auto playerInput   = scene_->GetComponent<PlayerInput>(playerEntity);

    // 入力がない場合はアイドル状態に遷移
    if (playerInput->GetInputDirection().lengthSq() <= 0.f) {
        return PlayerMoveState::IDLE;
    }
    // 地面にいる場合
    if (state->IsOnGround()) {
        if (playerInput->IsJumpInput()) {
            return PlayerMoveState::JUMP;
        }
    } else {
        // 空中にいる場合は落下状態に遷移
        return PlayerMoveState::FALL_DOWN;
    }

    return PlayerMoveState::DASH;
}
