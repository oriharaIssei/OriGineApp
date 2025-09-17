#include "PlayerIdleState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/effect/particle/emitter/Emitter.h"
#include "component/physics/Rigidbody.h"

#include "component/Player/PlayerInput.h"

void PlayerIdleState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto playerStatus  = scene_->getComponent<PlayerStatus>(playerEntity);
    auto rigidbody     = scene_->getComponent<Rigidbody>(playerEntity);
    rigidbody->setAcceleration({0.f, 0.0f, 0.0f});
    playerStatus->setCurrentMaxSpeed(0.0f);
    playerStatus->setGearUpCoolTime(playerStatus->getBaseGearupCoolTime());
    playerStatus->setGearLevel(kDefaultPlayerGearLevel);

    int32_t emitterSize = scene_->getComponentArray<Emitter>()->getComponentSize(playerEntity);

    //! TODO: 速度でパーティクルの量を変える
    for (int32_t i = 0; i < emitterSize; i++) {
        auto* emitter = scene_->getComponent<Emitter>(playerEntity, i);
        emitter->PlayStop();
    }
}

void PlayerIdleState::Update(float /*_deltaTime*/) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    // 減速
    rigidbody->setVelocity(rigidbody->getVelocity() - rigidbody->getVelocity() * 0.8f);
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
    auto playerStatus  = scene_->getComponent<PlayerStatus>(playerEntity);
    auto playerInput   = scene_->getComponent<PlayerInput>(playerEntity);

    if (playerStatus->isOnGround()) {
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
