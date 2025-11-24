#include "PlayerWallRunState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/player/PlayerEffectControlParam.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

#include "component/Camera/CameraController.h"

/// log
#include "logger/Logger.h"

/// math
#include "MyEasing.h"

void PlayerWallRunState::Initialize() {
    constexpr int32_t thresholdGearLevel = 2;

    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntity);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->GetComponent<Transform>(playerEntity);

    prevVelo_ = rigidbody->GetVelocity(); // 壁ジャンプ前の速度を保存

    // 進行方向を計算
    wallNormal_     = state->GetWallCollisionNormal();
    Vec3f direction = Vec3f::Cross(wallNormal_, axisY);
    if (direction.lengthSq() == 0.0f) {
        direction = Vec3f::Cross(wallNormal_, axisX);
    }
    direction = direction.normalize();

    if (Vec3f::Dot(direction, prevVelo_) < 0.0f) {
        direction = -direction;
    }

    playerSpeed_ = rigidbody->GetMaxXZSpeed();

    // 基準レベル未満なら レベル+1
    int32_t gearLevel = state->GetGearLevel();
    if (gearLevel < thresholdGearLevel) {
        auto& stateFlag = state->GetStateFlagRef();
        stateFlag.SetCurrent(stateFlag.Current() | PlayerStateFlag::GEAR_UP);

        int32_t addedGearLevel = state->GetGearLevel() + 1;
        state->SetGearLevel(addedGearLevel);

        playerStatus->SetGearUpCoolTime(playerStatus->CalculateCoolTimeByGearLevel(addedGearLevel));

        playerStatus->SetCurrentMaxSpeed(playerStatus->CalculateSpeedByGearLevel(addedGearLevel));
        rigidbody->SetMaxXZSpeed(playerStatus->GetCurrentMaxSpeed());

        playerSpeed_ = rigidbody->GetMaxXZSpeed();
    }

    // 移動方向を回転
    Vec3f velo = playerSpeed_ * direction;
    rigidbody->SetVelocity(velo);
    rigidbody->SetUseGravity(false); // 重力を無効

    // 壁との衝突判定の残り時間を初期化
    separationLeftTime_ = separationGraceTime_;

    // プレイヤーの向きを移動方向に合わせる
    PlayerEffectControlParam* effectParam = scene_->GetComponent<PlayerEffectControlParam>(playerEntity);
    Vec3f forward                         = direction;
    Quaternion lookForward                = Quaternion::LookAt(forward, axisY);
    float rotateOffsetOnWallRun           = effectParam->GetRotateOffsetOnWallRun();
    bool isRightWall                      = Vec3f::Dot(Vec3f::Cross(axisY, wallNormal_), direction) > 0.0f;
    Quaternion angleOffset                = Quaternion::RotateAxisAngle(forward, isRightWall ? rotateOffsetOnWallRun : -rotateOffsetOnWallRun);
    transform->rotate                     = lookForward * angleOffset;

    transform->UpdateMatrix();

    // speedRate_ と speedRumpUpTime_ をリセット
    speedRate_ = playerStatus->GetWallRunRate();

    speedRumpUpTime_  = playerStatus->GetWallRunRampUpTime();
    speedRumpUpTimer_ = 0.0f;

    // カメラのオフセットを計算
    Entity* cameraEntity               = scene_->GetUniqueEntity("GameCamera");
    CameraController* cameraController = scene_->GetComponent<CameraController>(cameraEntity);

    Vec3f targetTargetOffset = cameraController->GetTargetOffsetOnWallRun();
    cameraTargetOffsetOnWallRun_ =
        wallNormal_ * targetTargetOffset[X] // 横方向
        + axisY * targetTargetOffset[Y] // 上方向
        + direction * targetTargetOffset[Z]; // 前方向
    cameraTargetOffsetOnWallRun_ = cameraTargetOffsetOnWallRun_.normalize() * targetTargetOffset.length();

    Vec3f offsetOnWallRun = cameraController->GetOffsetOnWallRun();
    cameraOffsetOnWallRun_ =
        wallNormal_ * offsetOnWallRun[X] // 横方向
        + axisY * offsetOnWallRun[Y] // 上方向
        + direction * offsetOnWallRun[Z]; // 前方向
    cameraOffsetOnWallRun_ = cameraOffsetOnWallRun_.normalize() * offsetOnWallRun.length();

    // カメラの傾きを徐々に変えるためのタイマーをリセット
    cameraAngleLerpTimer_ = 0.0f;
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntity);
    auto* transform    = scene_->GetComponent<Transform>(playerEntity);

    // 衝突が途切れないようにめり込ませる
    transform->translate -= wallNormal_ * 0.1f;
    transform->UpdateMatrix();

    // 壁との衝突判定の残り時間を更新
    // これが0以下になると 壁から離れた と判定される
    if (state->IsCollisionWithWall()) {
        separationLeftTime_ = separationGraceTime_;
    } else {
        separationLeftTime_ -= _deltaTime;
    }

    // RumpUp 処理
    speedRumpUpTimer_ += _deltaTime;
    float rumpUpT           = speedRumpUpTimer_ / speedRumpUpTime_;
    rumpUpT                 = std::clamp(rumpUpT, 0.f, 1.f);
    float currentSpeedRate_ = std::lerp(1.f, speedRate_, EaseOutCubic(rumpUpT));
    // 速度を更新
    auto* rigidbody = scene_->GetComponent<Rigidbody>(playerEntity);
    Vec3f direction = rigidbody->GetVelocity().normalize();
    Vec3f newVelo   = direction * (playerSpeed_ * currentSpeedRate_);
    rigidbody->SetVelocity(newVelo);

    /// TODO: カメラの処理をここに書くべきではない
    // カメラの傾きを徐々に変える
    Entity* gameCameraEntity = scene_->GetUniqueEntity("GameCamera");
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

    CameraController* cameraController = scene_->GetComponent<CameraController>(gameCameraEntity);
    if (cameraController) {
        // カメラのオフセットを徐々に元に戻す
        const Vec3f& currentOffset = cameraController->GetCurrentOffset();
        Vec3f newOffset            = Lerp<3, float>(currentOffset, cameraOffsetOnWallRun_, EaseOutCubic(t));
        cameraController->SetCurrentOffset(newOffset);

        const Vec3f& currentTargetOffset = cameraController->GetCurrentTargetOffset();
        Vec3f newTargetOffset            = Lerp<3, float>(currentTargetOffset, cameraTargetOffsetOnWallRun_, EaseOutCubic(t));

        cameraController->SetCurrentTargetOffset(newTargetOffset);
    }
}

void PlayerWallRunState::Finalize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->GetComponent<Transform>(playerEntity);
    rigidbody->SetUseGravity(true); // 重力を有効

    transform->translate += wallNormal_ * 0.1f;

    transform->rotate = Quaternion::Identity();
}

PlayerMoveState PlayerWallRunState::TransitionState() const {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);

    if (separationLeftTime_ <= 0.0f) {
        return PlayerMoveState::FALL_DOWN;
    }

    auto playerInput = scene_->GetComponent<PlayerInput>(playerEntity);
    if (playerInput->IsJumpInput()) {
        return PlayerMoveState::WALL_JUMP;
    }

    return PlayerMoveState::WALL_RUN;
}
