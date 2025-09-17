#include "PlayerWallJumpState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"
#include "component/Player/PlayerInput.h"
#include "component/Stage.h"

void PlayerWallJumpState::Initialize() {
    constexpr float WALLJUMP_EPSILON_Y = 1.2f;
    auto* playerEntity                 = scene_->getEntity(playerEntityID_);
    auto* transform                    = scene_->getComponent<Transform>(playerEntity);
    auto* rigidbody                    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* playerStatus                 = scene_->getComponent<PlayerStatus>(playerEntity);

    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f}); // 壁ジャンプ時は加速度をリセット
    rigidbody->setUseGravity(false); // 無効
    prevVelo_ = rigidbody->getVelocity(); // 壁ジャンプ前の速度を保存

    // 目的のControlPoint への 差分ベクトルを進行方向とする
    Vec3f wallJumpDirection = nextControlPointPos(*transform, *rigidbody) - transform->translate;
    // 少し上にいくように誤差をプラス
    wallJumpDirection[Y] += WALLJUMP_EPSILON_Y;
    wallJumpDirection = wallJumpDirection.normalize();

    velo_ = wallJumpDirection * (rigidbody->getMaxXZSpeed() * playerStatus->getWallRunRate());

    leftTime_ = forcedJumpTime_; // 壁ジャンプの残り時間を初期化
}

void PlayerWallJumpState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    rigidbody->setVelocity(velo_); // 壁ジャンプの方向に速度を設定

    leftTime_ -= _deltaTime; // 壁ジャンプの残り時間を減少
}

void PlayerWallJumpState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    rigidbody->setUseGravity(true); // 重力を有効
    rigidbody->setVelocity(prevVelo_); // 壁ジャンプ終了時に速度をリセット
}

PlayerMoveState PlayerWallJumpState::TransitionState() const {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto playerStatus  = scene_->getComponent<PlayerStatus>(playerEntity);

    if (playerStatus->isOnGround()) {
        return PlayerMoveState::DASH;
    }
    if (leftTime_ <= 0.0f) {
        return PlayerMoveState::DASH;
    }

    return this->getState();
}

Vec3f PlayerWallJumpState::nextControlPointPos(const Transform& _playerTransform, const Rigidbody& _playerRigidbody) {
    Vec3f targetPointPos;

    GameEntity* stageEntity = scene_->getUniqueEntity("Stage");
    Stage* stage            = scene_->getComponent<Stage>(stageEntity);
    if (!stageEntity || !stage) {
        return targetPointPos;
    }

    Vec3f playerVeloNormal = _playerRigidbody.getVelocity().normalize();

    Vec3f* nearestLinkFromPos = nullptr;
    // Playerの位置と方向ベクトルから targetPointPos を取得
    for (const Stage::Link& link : stage->getLinks()) {
        Vec3f& linkFromPos = stage->getControlPointsRef()[link.from_].pos_;
        Vec3f diff         = linkFromPos - Vec3f(_playerTransform.worldMat[3]);

        // diffと playerVeloNormal の内積を計算
        // 内積が正の場合、playerVeloNormal と同じ方向にある
        float dot = diff.dot(playerVeloNormal);
        if (dot <= 0.0f) {
            continue;
        }

        if (nearestLinkFromPos) {
            // 既に最も近いリンクがある場合、距離を比較
            float currentDistance = diff.lengthSq();
            float nearestDistance = Vec3f(*nearestLinkFromPos - Vec3f(_playerTransform.worldMat[3])).lengthSq();
            if (currentDistance < nearestDistance) {
                nearestLinkFromPos = &linkFromPos; // より近いリンクを保存
            }
        } else {
            nearestLinkFromPos = &linkFromPos; // 最初のリンクを保存
        }
    }

    if (nearestLinkFromPos) {
        // 最も近いリンクが見つかった場合、その位置を targetPointPos に設定
        targetPointPos = *nearestLinkFromPos;
    } else {
        // 見つからなかった場合は、プレイヤーの位置をそのまま返す
        targetPointPos = Vec3f(_playerTransform.worldMat[3]);
    }
    return targetPointPos;
}
