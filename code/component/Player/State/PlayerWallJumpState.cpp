#include "PlayerWallJumpState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"
#include "component/Stage/Stage.h"
#include "component/Stage/StageWall.h"

/// log
#include "logger/Logger.h"

void PlayerWallJumpState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* playerState  = scene_->getComponent<PlayerState>(playerEntity);

    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f}); // 壁ジャンプ時は加速度をリセット
    rigidbody->setUseGravity(false); // 無効
    prevSpeed_ = rigidbody->getVelocity().length(); // 壁ジャンプ前の速度を保存

    // 目的のControlPoint への 差分ベクトルを進行方向とする
    Vec3f targetNormal = Vec3f(0.0f, 1.f, 0.f);

    GameEntity* wallEntity  = scene_->getEntity(playerState->getWallEntityIndex());
    Vec3f wallJumpDirection = Vec3f(0.0f, 1.f, 0.f);

    Vec3f nextPos     = nextControlPointPos(scene_->getComponent<StageWall>(wallEntity), targetNormal, transform, rigidbody);
    wallJumpDirection = nextPos - transform->translate;

    Vec3f jumpOffset = playerStatus->getWallJumpOffset() * MakeMatrix::RotateAxisAngle(axisY, targetNormal);
    wallJumpDirection += jumpOffset;
    wallJumpDirection = wallJumpDirection.normalize();

    velo_ = wallJumpDirection * (rigidbody->getMaxXZSpeed() * playerStatus->getWallRunRate());

    GameEntity* wallJumpTargetEntity   = scene_->getUniqueEntity("WallJumpTargetPos");
    auto wallJumpTargetTransform       = scene_->getComponent<Transform>(wallJumpTargetEntity);
    wallJumpTargetTransform->translate = nextPos + jumpOffset;
    wallJumpTargetTransform->UpdateMatrix();

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
    rigidbody->setVelocity(velo_.normalize() * prevSpeed_); // 壁ジャンプ終了時に速度をリセット
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

Vec3f PlayerWallJumpState::nextControlPointPos(const StageWall* _stageWall, Vec3f& _targetNormal, const Transform* _playerTransform, const Rigidbody* _playerRigidbody) const {
    Vec3f targetPointPos;

    GameEntity* stageEntity = scene_->getUniqueEntity("Stage");
    Stage* stage            = scene_->getComponent<Stage>(stageEntity);
    if (!stageEntity || !stage || !_stageWall) {
        LOG_WARN("Stage or StageWall is nullptr");
        return targetPointPos;
    }

    Vec3f playerVeloNormal = _playerRigidbody->getVelocity().normalize();

    Stage::Link* nearestLink = nullptr;
    Vec3f* nearestPoint      = nullptr;
    float nearLengthSq       = std::numeric_limits<float>::max();
    auto isNearestPoint      = [&nearestLink, &nearestPoint, &nearLengthSq, _playerTransform, &targetPointPos, &playerVeloNormal](Stage::Link& link, Vec3f& point) {
        Vec3f diff = point - Vec3f(_playerTransform->worldMat[3]);

        // diffと playerVeloNormal の内積を計算
        // 内積が正の場合、playerVeloNormal と同じ方向にある
        float dot = diff.normalize().dot(playerVeloNormal);
        if (dot <= 0.5f) {
            return;
        }

        // 既に最も近いリンクがある場合、距離を比較
        float currentDistance = diff.lengthSq();
        if (nearestLink && nearestPoint) {
            if (currentDistance < nearLengthSq) {
                nearestPoint = &point;
                nearestLink  = &link; // より近いリンクを保存
                nearLengthSq = currentDistance;
            }
        } else {
            nearestPoint = &point;
            nearestLink  = &link;
            nearLengthSq = currentDistance;
        }
    };

    // Playerの位置と方向ベクトルから targetPointPos を取得
    int32_t linkIndex = 0;

    for (Stage::Link& link : stage->getLinksRef()) {
        // 現在接触している壁のリンクは除外
        if (linkIndex == _stageWall->getLinkIndex()) {
            ++linkIndex;
            continue;
        }
        Vec3f& linkFromPos = stage->getControlPointsRef()[link.from_].pos_;
        Vec3f& linkToPos   = stage->getControlPointsRef()[link.to_].pos_;

        isNearestPoint(link, linkFromPos);
        isNearestPoint(link, linkToPos);

        ++linkIndex;
    }

    if (nearestLink && nearestPoint) {
        targetPointPos = *nearestPoint;
        _targetNormal  = nearestLink->normal_;
    } else {
        LOG_WARN("No valid target control point found for wall jump.");
        // 見つからなかった場合は、プレイヤーの位置をそのまま返す
        targetPointPos = Vec3f(_playerTransform->worldMat[3]);
    }
    return targetPointPos;
}
