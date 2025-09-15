#include "PlayerWallRunState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"
#include "component/Player/PlayerInput.h"

void PlayerWallRunState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    prevVelo_          = rigidbody->getVelocity(); // 壁ジャンプ前の速度を保存

    const Vec3f& normal = playerStatus->getWallCollisionNormal();
    Vec3f direction     = Vec3f::Cross(normal, Vec3f(0.0f, 1.0f, 0.0f));
    if (direction.lengthSq() == 0.0f) {
        direction = Vec3f::Cross(normal, Vec3f(1.0f, 0.0f, 0.0f));
    }
    direction = direction.normalize();
    if (Vec3f::Dot(direction, prevVelo_) < 0.0f) {
        direction = -direction;
    }
    // 移動方向を回転
    Vec3f velo = (playerStatus->getCurrentSpeed() * playerStatus->getWallRunRate()) * direction;
    rigidbody->setVelocity(velo);

    separationdLeftTime_ = separationGraceTime_; // 壁との衝突判定の残り時間を初期化
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);

    if (playerStatus->isCollisionWithWall()) {
        separationdLeftTime_ = separationGraceTime_;
    } else {
        separationdLeftTime_ -= _deltaTime; // 壁との衝突判定の残り時間を減少
    }
}

void PlayerWallRunState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    // 壁走行終了時に速度をリセット
    rigidbody->setVelocity(prevVelo_); // 壁走行終了時に速度をリセット

    auto playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    playerStatus->setCollisionWithWall(false); // 壁走行終了時に壁との衝突をリセット
}

PlayerMoveState PlayerWallRunState::TransitionState() const {
    auto* playerEntity = scene_->getEntity(playerEntityID_);

    if (separationdLeftTime_ <= 0.0f) {
        return PlayerMoveState::FALL_DOWN;
    }

    auto playerInput = scene_->getComponent<PlayerInput>(playerEntity);
    if (playerInput->isJumpInput()) {
        return PlayerMoveState::WALL_JUMP;
    }

    return PlayerMoveState::WALL_RUN;
}
