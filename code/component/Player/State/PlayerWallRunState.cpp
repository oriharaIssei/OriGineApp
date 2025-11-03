#include "PlayerWallRunState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/Player/PlayerEffectControlParam.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerState.h"

#include "component/Camera/CameraController.h"

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

    // プレイヤーの向きを移動方向に合わせる
    PlayerEffectControlParam* effectParam = scene_->getComponent<PlayerEffectControlParam>(playerEntity);
    Vec3f forward                         = direction;
    Quaternion lookForward                = Quaternion::LookAt(forward, axisY);
    float rotateOffsetOnWallRun           = effectParam->getRotateOffsetOnWallRun();
    bool isRightWall                      = Vec3f::Dot(Vec3f::Cross(axisY, wallNormal_), direction) > 0.0f;
    Quaternion angleOffset                = Quaternion::RotateAxisAngle(forward, isRightWall ? rotateOffsetOnWallRun : -rotateOffsetOnWallRun);
    transform->rotate                     = lookForward * angleOffset;

    transform->UpdateMatrix();

    // カメラのオフセットを計算
    Entity* cameraEntity               = scene_->getUniqueEntity("GameCamera");
    CameraController* cameraController = scene_->getComponent<CameraController>(cameraEntity);

    Vec3f targetTargetOffset = cameraController->getTargetOffsetOnWallRun();
    cameraTargetOffsetOnWallRun_ =
        wallNormal_ * targetTargetOffset[X] // 横方向
        + axisY * targetTargetOffset[Y] // 上方向
        + direction * targetTargetOffset[Z]; // 前方向
    cameraTargetOffsetOnWallRun_ = cameraTargetOffsetOnWallRun_.normalize() * targetTargetOffset.length();

    Vec3f offsetOnWallRun = cameraController->getOffsetOnWallRun();
    cameraOffsetOnWallRun_ =
        wallNormal_ * offsetOnWallRun[X] // 横方向
        + axisY * offsetOnWallRun[Y] // 上方向
        + direction * offsetOnWallRun[Z]; // 前方向
    cameraOffsetOnWallRun_ = cameraOffsetOnWallRun_.normalize() * offsetOnWallRun.length();

    // カメラの傾きを徐々に変えるためのタイマーをリセット
    cameraAngleLerpTimer_ = 0.0f;
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* state        = scene_->getComponent<PlayerState>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);

    // 衝突が途切れないようにめり込ませる
    transform->translate -= wallNormal_ * 0.1f;
    transform->UpdateMatrix();

    if (state->isCollisionWithWall()) {
        separationLeftTime_ = separationGraceTime_;
    } else {
        separationLeftTime_ -= _deltaTime; // 壁との衝突判定の残り時間を減少
    }

    /// TODO: カメラの処理をここに書くべきではない
    // カメラの傾きを徐々に変える
    Entity* gameCameraEntity = scene_->getUniqueEntity("GameCamera");
    if (!gameCameraEntity) {
        return;
    }

    cameraAngleLerpTimer_ += _deltaTime;
    float t = cameraAngleLerpTimer_ / kCameraAngleLerpTime_;
    t       = std::clamp(t, 0.f, 1.f);

    // 一度だけ実行
    if (t >= 1) {
        return;
    }

    CameraController* cameraController = scene_->getComponent<CameraController>(gameCameraEntity);
    if (cameraController) {
        // カメラのオフセットを徐々に元に戻す
        const Vec3f& currentOffset = cameraController->getCurrentOffset();
        Vec3f newOffset            = Lerp<3, float>(currentOffset, cameraOffsetOnWallRun_, EaseOutCubic(t));
        cameraController->setCurrentOffset(newOffset);

        const Vec3f& currentTargetOffset = cameraController->getCurrentTargetOffset();
        Vec3f newTargetOffset            = Lerp<3, float>(currentTargetOffset, cameraTargetOffsetOnWallRun_, EaseOutCubic(t));

        cameraController->setCurrentTargetOffset(newTargetOffset);
    }
}

void PlayerWallRunState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);
    rigidbody->setUseGravity(true); // 重力を有効

    transform->translate += wallNormal_ * 0.1f;

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
