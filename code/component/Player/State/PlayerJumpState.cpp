#include "PlayerJumpState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"
#include "component/Player/PlayerInput.h"

void PlayerJumpState::Initialize() {
    releaseJumpPower_ = 0.f;

    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto playerStatus  = scene_->getComponent<PlayerStatus>(playerEntity);

    rigidbody->setUseGravity(false);
    rigidbody->setVelocity(Y, playerStatus->getJumpPower()); // ジャンプパワーをY軸に設定

    auto* skinningAnim = scene_->getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->endTransition();
        skinningAnim->Play(2);
        // ジャンプアップのアニメーションを再生
        skinningAnim->PlayNext(3); // 2 はジャンプアップのアニメーションインデックス
    }
}

void PlayerJumpState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* playerInput  = scene_->getComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);

    playerStatus->UpdateAccel(playerInput, transform, rigidbody, scene_->getComponent<CameraTransform>(scene_->getUniqueEntity("GameCamera"))->rotate);

    releaseJumpPower_ += playerStatus->getFallPower();
}

void PlayerJumpState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* skinningAnim = scene_->getComponent<SkinningAnimationComponent>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    rigidbody->setUseGravity(true);

    rigidbody->setVelocity(Y, releaseJumpPower_);

    if (skinningAnim) {
        skinningAnim->Play(4); // 差し替え
    }
}

PlayerMoveState PlayerJumpState::TransitionState() const {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto playerStatus  = scene_->getComponent<PlayerStatus>(playerEntity);
    auto playerInput   = scene_->getComponent<PlayerInput>(playerEntity);

    if (playerStatus->isOnGround()) {
        if (playerStatus->isCollisionWithWall()) {
            return PlayerMoveState::WALL_RUN;
        } else {
            if (playerInput->getInputDirection().lengthSq() > 0.f) {
                return PlayerMoveState::DASH;
            }
        }

        return PlayerMoveState::IDLE;
    } else {
        if (!playerInput->isJumpInput()) {
            // ジャンプ入力がない場合は落下状態に遷移
            return PlayerMoveState::FALL_DOWN;
        }
    }

    return PlayerMoveState::JUMP;
}
