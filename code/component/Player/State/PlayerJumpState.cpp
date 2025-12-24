#include "PlayerJumpState.h"

/// component
#include "component/physics/Rigidbody.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/State/PlayerState.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

///  math
#include "math/MyEasing.h"

using namespace OriGine;

void PlayerJumpState::Initialize() {
    releaseJumpPower_ = 0.f;

    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);

    auto* playerState = scene_->GetComponent<PlayerState>(playerEntityHandle_);

    rigidbody->SetUseGravity(false);

    float t      = static_cast<float>(playerState->GetGearLevel()) / static_cast<float>(kMaxPlayerGearLevel);
    float easedT = EasingFunctions[static_cast<int32_t>(playerStatus->GetJumpHoldVelocityEaseType())](t);

    rigidbody->SetVelocity(Y, std::lerp(playerStatus->GetMinJumpHoldVelocity(), playerStatus->GetMaxJumpHoldVelocity(), t)); // ジャンプパワーをY軸に設定

    easedT       = EasingFunctions[static_cast<int32_t>(playerStatus->GetJumpChargeRateEaseType())](t);
    chargePower_ = std::lerp(playerStatus->GetMinJumpChargeRate(), playerStatus->GetMaxJumpChargeRate(), easedT);
}

void PlayerJumpState::Update(float _deltaTime) {
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* playerInput  = scene_->GetComponent<PlayerInput>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);


    CameraTransform* cameraTransform = scene_->GetComponent<CameraTransform>(state->GetCameraEntityHandle());

    // 移動処理
    playerStatus->UpdateAccel(_deltaTime, playerInput, transform, rigidbody, cameraTransform->rotate);

    // ジャンプ力の蓄積
    releaseJumpPower_ += chargePower_ * _deltaTime;
}

void PlayerJumpState::Finalize() {
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);

    rigidbody->SetUseGravity(true);
    // 蓄えたジャンプ力を適用
    rigidbody->SetVelocity(Y, rigidbody->GetVelocity()[Y] + releaseJumpPower_);
}

PlayerMoveState PlayerJumpState::TransitionState() const {
    auto state         = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto playerInput   = scene_->GetComponent<PlayerInput>(playerEntityHandle_);

    if (state->IsCollisionWithWall()) {
        return PlayerMoveState::WALL_RUN;
    }

    if (state->IsOnGround()) {
        if (playerInput->GetInputDirection().lengthSq() > 0.f) {
            return PlayerMoveState::DASH;
        }
        return PlayerMoveState::IDLE;
    } else {
        if (!playerInput->IsJumpInput()) {
            // ジャンプ入力がない場合は落下状態に遷移
            return PlayerMoveState::FALL_DOWN;
        }
    }

    return PlayerMoveState::JUMP;
}
