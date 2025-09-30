#include "PlayerWallJumpState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/collider/Collider.h"
#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"
#include "component/Stage/Stage.h"
#include "component/Stage/StageWall.h"

/// log
#include "logger/Logger.h"

void PlayerWallJumpState::Initialize() {
    auto* playerEntity             = scene_->getEntity(playerEntityID_);
    auto* transform                = scene_->getComponent<Transform>(playerEntity);
    auto* rigidbody                = scene_->getComponent<Rigidbody>(playerEntity);
    auto* playerStatus             = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* playerState              = scene_->getComponent<PlayerState>(playerEntity);
    SphereCollider* playerCollider = scene_->getComponent<SphereCollider>(playerEntity);

    /// ========================================
    // 速度の初期化
    /// ========================================
    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f}); // 壁ジャンプ時は加速度をリセット
    rigidbody->setUseGravity(false); // 無効
    prevSpeed_ = rigidbody->getVelocity().length(); // 壁ジャンプ前の速度を保存

    /// ========================================
    // 目的地を決め, 方向を決める
    /// ========================================
    // 目的のControlPoint への 差分ベクトルを進行方向とする
    Vec3f targetNormal = Vec3f(0.0f, 1.f, 0.f);

    GameEntity* wallEntity  = scene_->getEntity(playerState->getWallEntityIndex());
    Vec3f wallJumpDirection = Vec3f(0.0f, 1.f, 0.f);

    Vec3f nextPos     = nextControlPointPos(scene_->getComponent<StageWall>(wallEntity), targetNormal, transform, rigidbody);
    wallJumpDirection = nextPos - (transform->translate + playerCollider->getLocalCenter());

    Vec3f forward = rigidbody->getVelocity().normalize();
    Vec3f right   = Vec3f::Cross(targetNormal, forward).normalize();
    forward       = Vec3f::Cross(right, targetNormal).normalize();
    Matrix4x4 rotateMat =
        {right[X], right[Y], right[Z], 0.0f,
            targetNormal[X], targetNormal[Y], targetNormal[Z], 0.0f,
            forward[X], forward[Y], forward[Z], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

    Vec3f jumpOffset = playerStatus->getWallJumpOffset() * rotateMat;
    wallJumpDirection += jumpOffset;
    wallJumpDirection = wallJumpDirection.normalize();

    velo_ = wallJumpDirection * (rigidbody->getMaxXZSpeed() * playerStatus->getWallRunRate());

#ifndef _RELEASE
    GameEntity* wallJumpTargetEntity   = scene_->getUniqueEntity("WallJumpTargetPos");
    auto wallJumpTargetTransform       = scene_->getComponent<Transform>(wallJumpTargetEntity);
    wallJumpTargetTransform->translate = nextPos + jumpOffset;
    wallJumpTargetTransform->UpdateMatrix();
#endif // DEBUG

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
    constexpr Vec3f kDistanceWeight = Vec3f(0.9f, 1.15f, 1.05f);

    Vec3f targetPointPos;

    GameEntity* stageEntity = scene_->getUniqueEntity("Stage");
    Stage* stage            = scene_->getComponent<Stage>(stageEntity);
    if (!stageEntity || !stage || !_stageWall) {
        LOG_WARN("Stage or StageWall is nullptr");
        return targetPointPos;
    }

    Vec3f refPos           = stage->getControlPoints()[_stageWall->getToPointIndex()].pos_;
    Vec3f playerVeloNormal = _playerRigidbody->getVelocity().normalize();

    Stage::Link* nearestLink = nullptr;
    Vec3f* nearestPoint      = nullptr;
    float nearLengthSq       = std::numeric_limits<float>::max();
    auto isNearestPoint      = [&nearestLink, &nearestPoint, &nearLengthSq, &targetPointPos, &playerVeloNormal](const Vec3f& _refPoint, Stage::Link& link, Vec3f& point) {
        Vec3f diff = point - _refPoint;

        diff = diff * kDistanceWeight; // Y軸を少し伸ばして距離を測る

        // diffと playerVeloNormal の内積を計算
        // 内積が正の場合、playerVeloNormal と同じ方向にある
        float dot = diff.normalize().dot(playerVeloNormal);
        if (dot <= 0.6f) {
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

        isNearestPoint(refPos, link, linkFromPos);
        isNearestPoint(refPos, link, linkToPos);

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
