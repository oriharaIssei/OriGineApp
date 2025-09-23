#include "PlayerWallRunState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/Player/PlayerInput.h"

#include "component/Stage/Stage.h"
#include "component/Stage/StageWall.h"

void PlayerWallRunState::Initialize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* state        = scene_->getComponent<PlayerState>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);

    prevVelo_ = rigidbody->getVelocity(); // 壁ジャンプ前の速度を保存

    wallNormal_     = state->getWallCollisionNormal();
    Vec3f direction = Vec3f::Cross(wallNormal_, axisY);
    if (direction.lengthSq() == 0.0f) {
        direction = Vec3f::Cross(wallNormal_, axisX);
    }

    direction = direction.normalize();
    if (Vec3f::Dot(direction, prevVelo_) < 0.0f) {
        direction = -direction;
    }

    // 壁の情報を取得
    StageWall* wall = scene_->getComponent<StageWall>(scene_->getEntity(state->getWallEntityIndex()));
    getWallData(wall);
    // 壁走りの高さを計算
    wallRunHeight_  = calculateWallRunHeight(transform);
    currentHeight_  = transform->translate[Y];

    // 移動方向を回転
    Vec3f velo = rigidbody->getMaxXZSpeed() * direction;
    rigidbody->setVelocity(velo);
    rigidbody->setUseGravity(false); // 重力を無効

    // 回転
    Quaternion frontQuat  = Quaternion::RotateAxisVector(axisY, direction).normalize();
    Quaternion toWallQuat = Quaternion::RotateAxisVector(axisX, wallNormal_).normalize();
    transform->rotate     = Quaternion::Normalize(toWallQuat * frontQuat);

    separationLeftTime_ = separationGraceTime_; // 壁との衝突判定の残り時間を初期化
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* state        = scene_->getComponent<PlayerState>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);

    wallRunHeight_ = calculateWallRunHeight(transform);

    climbTimer_ += _deltaTime;
    climbTimer_ = std::min(climbTimer_, climbTime_);
    float t     = climbTimer_ / climbTime_;

    transform->translate[Y] = std::lerp(currentHeight_, wallRunHeight_, t);
    transform->Update();

    if (state->isCollisionWithWall()) {
        separationLeftTime_ = separationGraceTime_;
    } else {
        separationLeftTime_ -= _deltaTime; // 壁との衝突判定の残り時間を減少
    }
}

void PlayerWallRunState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    rigidbody->setUseGravity(true); // 重力を有効
    // 壁走行終了時に速度をリセット
    rigidbody->setVelocity(prevVelo_); // 壁走行終了時に速度をリセット
}

PlayerMoveState PlayerWallRunState::TransitionState() const {
    auto* playerEntity = scene_->getEntity(playerEntityID_);

    if (separationLeftTime_ <= 0.0f) {
        return PlayerMoveState::FALL_DOWN;
    }

    auto playerInput = scene_->getComponent<PlayerInput>(playerEntity);
    if (playerInput->isJumpInput()) {
        return PlayerMoveState::WALL_JUMP;
    }

    return PlayerMoveState::WALL_RUN;
}

void PlayerWallRunState::getWallData(StageWall* _wall) {
    if (_wall == nullptr) {
        return;
    }

    Stage* stage = scene_->getComponent<Stage>(scene_->getUniqueEntity("Stage"));
    if (stage == nullptr) {
        return;
    }

    wallStartPos_ = stage->getControlPoints()[_wall->getFromPointIndex()].pos_;
    wallEndPos_   = stage->getControlPoints()[_wall->getToPointIndex()].pos_;
}

float PlayerWallRunState::calculateWallRunHeight(Transform* _transform) {
    Vec3f playerPos(_transform->worldMat[3]);

    // 壁の上下方向ベクトル
    Vec3f wallDir    = wallEndPos_ - wallStartPos_;
    float wallLength = wallDir.length();
    if (wallLength < std::numeric_limits<float>::epsilon()) {
        return 0.0f; // 高さがゼロなら抜ける
    }

    // 正規化
    Vec3f wallUp = wallDir / wallLength;

    // playerPos を wallStartPos_ から wallDir に射影 → パラメータ t
    float t = Vec3f(playerPos - wallStartPos_).dot(wallUp) / wallLength;

    // clamp して 0.0 ≤ t ≤ 1.0 に収める（線分内に制限）
    t = std::clamp(t, 0.0f, 1.0f);

    // t に対応する座標を取得
    Vec3f onWallPos = Lerp(wallStartPos_, wallEndPos_, t);

    return onWallPos[Y];
}
