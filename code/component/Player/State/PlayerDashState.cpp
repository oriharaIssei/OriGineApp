#include "PlayerDashState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"
#include "component/Player/PlayerInput.h"

void PlayerDashState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);

    auto* skinningAnim = scene_->getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->setPlaybackSpeed(1, 1.f + float(playerStatus->getGearLevel()));
    }
}

void PlayerDashState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* playerInput  = scene_->getComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);

    /// ---------------------------------------------------------------------
    // gearLevel の更新
    playerStatus->minusGearUpCoolTime(_deltaTime);

    if (playerStatus->getGearLevel() < playerStatus->getMaxGearLevel()) {
        if (playerStatus->getGearUpCoolTime() <= 0.f) {
            playerStatus->setGearUp(true);

            int32_t addedGearLevel = playerStatus->getGearLevel() + 1;
            playerStatus->setGearLevel(addedGearLevel);

            playerStatus->setGearUpCoolTime(playerStatus->CalculateCoolTimeByGearLevel(addedGearLevel));

            playerStatus->setCurrentMaxSpeed(playerStatus->CalculateSpeedByGearLevel(addedGearLevel));

            auto* skinningAnim = scene_->getComponent<SkinningAnimationComponent>(playerEntity);
            if (skinningAnim) {
                skinningAnim->setPlaybackSpeed(1, 1.f + float(playerStatus->getGearLevel()));
            }
        }
    }

   playerStatus->UpdateVelocity(playerInput, transform, rigidbody, scene_->getComponent<CameraTransform>(scene_->getUniqueEntity("GameCamera"))->rotate,_deltaTime);
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
    auto playerStatus  = scene_->getComponent<PlayerStatus>(playerEntity);
    auto playerInput   = scene_->getComponent<PlayerInput>(playerEntity);

    if (playerInput->getInputDirection().lengthSq() <= 0.f) {
        return PlayerMoveState::IDLE;
    }

    if (playerStatus->isOnGround()) {
        if (playerInput->isJumpInput()) {
            return PlayerMoveState::JUMP;
        }
    } else {
        // 空中にいる場合はジャンプ状態に遷移
        return PlayerMoveState::FALL_DOWN;
    }

    return PlayerMoveState::DASH;
}
