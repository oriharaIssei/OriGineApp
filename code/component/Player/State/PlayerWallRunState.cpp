#include "PlayerWallRunState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"

#include "component/Camera/CameraController.h"

#include "component/Stage/Stage.h"
#include "component/Stage/StageWall.h"

/// log
#include "logger/Logger.h"

/// math
#include "MyEasing.h"

void PlayerWallRunState::Initialize() {
    constexpr int32_t thresholdGearLevel = 2;

    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
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
    wallRunHeight_ = calculateWallRunHeight(transform);
    currentHeight_ = transform->translate[Y];

    float speed = rigidbody->getMaxXZSpeed();

    // 基準レベル未満なら レベル+1
    int32_t gearLevel = state->getGearLevel();
    if (gearLevel < thresholdGearLevel) {
        state->setGearUp(true);

        int32_t addedGearLevel = state->getGearLevel() + 1;
        state->setGearLevel(addedGearLevel);

        playerStatus->setGearUpCoolTime(playerStatus->CalculateCoolTimeByGearLevel(addedGearLevel));

        playerStatus->setCurrentMaxSpeed(playerStatus->CalculateSpeedByGearLevel(addedGearLevel));
        rigidbody->setMaxXZSpeed(playerStatus->getCurrentMaxSpeed());

        speed = rigidbody->getMaxXZSpeed();
    }

    // 移動方向を回転
    Vec3f velo = speed * direction;
    rigidbody->setVelocity(velo);
    rigidbody->setUseGravity(false); // 重力を無効

    // 壁との衝突判定の残り時間を初期化
    separationLeftTime_ = separationGraceTime_;

    // forward, right, up から回転を作る
    float angle       = std::atan2(-wallNormal_[X], wallNormal_[Y]);
    transform->rotate = Quaternion::RotateAxisAngle(axisZ, angle);
    transform->UpdateMatrix();

    cameraRotateSigne_ = wallNormal_[X] < 0.0f ? -1.0f : 1.0f;

    // 壁を登るタイマーをリセット
    climbTimer_ = 0.0f;
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* state        = scene_->getComponent<PlayerState>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);

    wallRunHeight_ = calculateWallRunHeight(transform);

    climbTimer_ += _deltaTime;
    climbTimer_ = std::min(climbTimer_, climbTime_);
    float t     = climbTimer_ / climbTime_;

    // 段々上に行く
    transform->translate[Y] = std::lerp(currentHeight_, wallRunHeight_, t);
    // 衝突が途切れないようにめり込ませる
    transform->translate[X] -= wallNormal_[X] * 0.1f;
    transform->UpdateMatrix();

    if (state->isCollisionWithWall()) {
        separationLeftTime_ = separationGraceTime_;
    } else {
        separationLeftTime_ -= _deltaTime; // 壁との衝突判定の残り時間を減少
    }

    /// TODO: カメラの処理をここに書くべきではない
    // カメラの傾きを徐々に変える
    CameraController* cameraController = scene_->getComponent<CameraController>(scene_->getUniqueEntity("GameCamera"));

    if (cameraController) {
        // カメラのオフセットを徐々に元に戻す
        cameraAngleLerpTimer_ += _deltaTime;
        float t = cameraAngleLerpTimer_ / kCameraAngleLerpTime_;
        t       = std::clamp(t, 0.f, 1.f);

        Vec3f targetOffset         = cameraController->getOffsetOnWallRun();
        const Vec3f& currentOffset = cameraController->getCurrentOffset();
        targetOffset[X] *= cameraRotateSigne_; // 壁の向きに合わせて左右反転
        Vec3f newOffset = Lerp<3, float>(currentOffset, targetOffset, EaseOutCubic(t));
        cameraController->setCurrentOffset(newOffset);

        Vec3f targetTargetOffset         = cameraController->getTargetOffsetOnWallRun();
        const Vec3f& currentTargetOffset = cameraController->getCurrentTargetOffset();
        targetTargetOffset[X] *= cameraRotateSigne_; // 壁の向きに合わせて左右反転
        Vec3f newTargetOffset = Lerp<3, float>(currentTargetOffset, targetTargetOffset, EaseOutCubic(t));
        cameraController->setCurrentTargetOffset(newTargetOffset);
    }
}

void PlayerWallRunState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);
    rigidbody->setUseGravity(true); // 重力を有効

    transform->rotate = Quaternion::Identity();
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
        LOG_WARN("Wall is nullptr.");
        return;
    }

    Stage* stage = scene_->getComponent<Stage>(scene_->getUniqueEntity("Stage"));
    if (stage == nullptr) {
        LOG_WARN("Stage is nullptr.");
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
    Vec3f onWallPos = Lerp(wallStartPos_, wallEndPos_, EaseOutCubic(t));

    return onWallPos[Y];
}
