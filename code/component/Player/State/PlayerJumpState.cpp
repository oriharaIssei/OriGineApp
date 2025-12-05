#include "PlayerJumpState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"
#include "PlayerState.h"

void PlayerJumpState::Initialize() {
    releaseJumpPower_ = 0.f;

    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);
    auto playerStatus  = scene_->GetComponent<PlayerStatus>(playerEntity);

    rigidbody->SetUseGravity(false);
    rigidbody->SetVelocity(Y, playerStatus->GetJumpPower()); // ジャンプパワーをY軸に設定
}

void PlayerJumpState::Update(float _deltaTime) {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* playerInput  = scene_->GetComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->GetComponent<Transform>(playerEntity);

    // 移動処理
    playerStatus->UpdateAccel(_deltaTime, playerInput, transform, rigidbody, scene_->GetComponent<CameraTransform>(scene_->GetUniqueEntity("GameCamera"))->rotate);

    // ジャンプ力の蓄積
    releaseJumpPower_ += playerStatus->GetFallPower() * _deltaTime;
}

void PlayerJumpState::Finalize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);

    rigidbody->SetUseGravity(true);
    // 蓄えたジャンプ力を適用
    rigidbody->SetVelocity(Y, releaseJumpPower_);
}

PlayerMoveState PlayerJumpState::TransitionState() const {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto state         = scene_->GetComponent<PlayerState>(playerEntity);
    auto playerInput   = scene_->GetComponent<PlayerInput>(playerEntity);

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
