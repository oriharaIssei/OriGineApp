#include "PlayerWallJumpState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"
#include "component/Stage.h"

void PlayerWallJumpState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);

    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f}); // 壁ジャンプ時は加速度をリセット
    rigidbody->setUseGravity(false); // 無効
    prevVelo_ = rigidbody->getVelocity(); // 壁ジャンプ前の速度を保存

    // 目的のControlPoint への 差分ベクトルを進行方向とする
    Vec3f targetNormal = Vec3f(0.0f, 1.f, 0.f);

    Vec3f wallJumpDirection = nextControlPointPos(targetNormal, transform, rigidbody) - transform->translate;

    Vec3f jumpOffset = playerStatus->getWallJumpOffset() * MakeMatrix::RotateAxisAngle(axisZ, targetNormal);
    wallJumpDirection += jumpOffset;
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
    auto state         = scene_->getComponent<PlayerState>(playerEntity);

    if (state->isOnGround()) {
        return PlayerMoveState::DASH;
    }
    if (leftTime_ <= 0.0f) {
        return PlayerMoveState::DASH;
    }

    return this->getState();
}

Vec3f PlayerWallJumpState::nextControlPointPos(Vec3f& _targetNormal, const Transform* _playerTransform, const Rigidbody* _playerRigidbody) const {
    Vec3f targetPointPos;

    GameEntity* stageEntity = scene_->getUniqueEntity("Stage");
    Stage* stage            = scene_->getComponent<Stage>(stageEntity);
    if (!stageEntity || !stage) {
        return targetPointPos;
    }

    Vec3f playerVeloNormal = _playerRigidbody->getVelocity().normalize();

    Stage::Link* nearestLink = nullptr;
    Vec3f* nearestPoint      = nullptr;
    auto isNearestPoint      = [&nearestLink, &nearestPoint, _playerTransform, &targetPointPos, &playerVeloNormal](Stage::Link& link, Vec3f& point) {
        Vec3f diff = point - Vec3f(_playerTransform->worldMat[3]);

        // diffと playerVeloNormal の内積を計算
        // 内積が正の場合、playerVeloNormal と同じ方向にある
        float dot = diff.dot(playerVeloNormal);
        if (dot <= 0.0f) {
            return;
        }

        // 既に最も近いリンクがある場合、距離を比較
        float currentDistance = diff.lengthSq();
        if (nearestLink && nearestPoint) {
            if (currentDistance * currentDistance < nearestPoint->lengthSq()) {
                nearestPoint = &point;
                nearestLink  = &link; // より近いリンクを保存
            }
        } else {
            nearestPoint = &point;
            nearestLink  = &link; // より近いリンクを保存
        }
    };

    // Playerの位置と方向ベクトルから targetPointPos を取得
    for (Stage::Link& link : stage->getLinksRef()) {
        Vec3f& linkFromPos = stage->getControlPointsRef()[link.from_].pos_;
        Vec3f& linkToPos   = stage->getControlPointsRef()[link.to_].pos_;

        isNearestPoint(*nearestLink, linkFromPos);
        isNearestPoint(*nearestLink, linkToPos);
    }

    if (nearestLink && nearestPoint) {
        targetPointPos = *nearestPoint;
        _targetNormal  = nearestLink->normal_;
    } else {
        // 見つからなかった場合は、プレイヤーの位置をそのまま返す
        targetPointPos = Vec3f(_playerTransform->worldMat[3]);
    }
    return targetPointPos;
}
