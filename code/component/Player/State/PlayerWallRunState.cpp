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
#include "math/mathEnv.h"
#include "MyEasing.h"

using namespace OriGine;

void PlayerWallRunState::Initialize() {
    constexpr int32_t thresholdGearLevel = 3;

    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntity);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntity);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntity);

    // 壁ジャンプ前の速度
    prevVelo_ = rigidbody->GetVelocity();

    // 壁法線
    wallNormal_ = state->GetWallCollisionNormal().normalize();

    // ===== 進行方向を「速度を壁面に投影して」求める =====
    OriGine::Vec3f direction =
        prevVelo_ - wallNormal_ * OriGine::Vec3f::Dot(prevVelo_, wallNormal_);
    direction[Y] = 0.0f;

    if (direction.lengthSq() > kEpsilon) {
        direction = direction.normalize();
    } else {
        // フォールバック（ほぼ起きない）
        direction = axisZ * MakeMatrix4x4::RotateY(transform->rotate.ToYaw());
    }

    // ===== スピード =====
    playerSpeed_ = playerStatus->CalculateSpeedByGearLevel(state->GetGearLevel());

    // 基準レベル未満ならギアアップ
    int32_t gearLevel = state->GetGearLevel();
    if (gearLevel < thresholdGearLevel) {
        auto& stateFlag = state->GetStateFlagRef();
        stateFlag.SetCurrent(stateFlag.Current() | PlayerStateFlag::GEAR_UP);

        int32_t addedGearLevel = gearLevel + 1;
        state->SetGearLevel(addedGearLevel);

        playerStatus->SetGearUpCoolTime(
            playerStatus->CalculateCoolTimeByGearLevel(addedGearLevel));

        playerStatus->SetCurrentMaxSpeed(
            playerStatus->CalculateSpeedByGearLevel(addedGearLevel));

        rigidbody->SetMaxXZSpeed(playerStatus->GetCurrentMaxSpeed());
        playerSpeed_ = rigidbody->GetMaxXZSpeed();
    }

    // ===== 移動 =====
    rigidbody->SetVelocity(playerSpeed_ * direction);
    rigidbody->SetUseGravity(false);

    // 壁との分離猶予
    separationLeftTime_ = separationGraceTime_;

    // ===== 向きとロール =====
    PlayerEffectControlParam* effectParam =
        scene_->GetComponent<PlayerEffectControlParam>(playerEntity);

    float rotateZOffsetOnWallRun = effectParam->GetRotateOffsetOnWallRun();

    // プレイヤーの向きを移動方向に合わせる
    Quaternion lookForward = Quaternion::LookAt(direction, axisY);
    bool isRightWall       = Vec3f::Dot(Vec3f::Cross(axisY, wallNormal_), direction) > 0.0f;
    Quaternion angleOffset = Quaternion::RotateAxisAngle(axisZ, isRightWall ? rotateZOffsetOnWallRun : -rotateZOffsetOnWallRun);
    transform->rotate      = lookForward * angleOffset;

    // ===== スピード制御 =====
    speedRate_        = playerStatus->GetWallRunRate();
    speedRumpUpTime_  = playerStatus->GetWallRunRampUpTime();
    speedRumpUpTimer_ = 0.0f;

    // ===== カメラ =====
    OriGine::Entity* cameraEntity =
        scene_->GetEntity(state->GetCameraEntityID());

    CameraController* cameraController =
        scene_->GetComponent<CameraController>(cameraEntity);

    cameraController->SetDestinationAngleXY(Vec2f(0.f, 0.f));

    // 左壁想定のオフセットを取得
    cameraTargetOffsetOnWallRun_ =
        cameraController->GetTargetOffsetOnWallRun();

    cameraOffsetOnWallRun_ =
        cameraController->GetOffsetOnWallRun();

    // ===== 右壁なら左右反転 =====
    if (isRightWall) {
        cameraTargetOffsetOnWallRun_[X] *= -1.0f;
        cameraOffsetOnWallRun_[X] *= -1.0f;
    }

    cameraAngleLerpTimer_ = 0.0f;
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntity);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntity);

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
    float rumpUpT          = speedRumpUpTimer_ / speedRumpUpTime_;
    rumpUpT                = std::clamp(rumpUpT, 0.f, 1.f);
    float currentSpeedRate = std::lerp(1.f, speedRate_, EaseOutCubic(rumpUpT));
    // 速度を更新
    auto* rigidbody          = scene_->GetComponent<Rigidbody>(playerEntity);
    OriGine::Vec3f direction = rigidbody->GetVelocity().normalize();
    OriGine::Vec3f newVelo   = direction * (playerSpeed_ * currentSpeedRate);
    rigidbody->SetVelocity(newVelo);
    rigidbody->SetMaxXZSpeed(newVelo.length());

    /// TODO: カメラの処理をここに書くべきではない
    // カメラの傾きを徐々に変える
    OriGine::Entity* gameCameraEntity = scene_->GetEntity(state->GetCameraEntityID());
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
        const OriGine::Vec3f& currentOffset = cameraController->GetCurrentOffset();
        OriGine::Vec3f newOffset            = Lerp<3, float>(currentOffset, cameraOffsetOnWallRun_, EaseOutCubic(t));
        cameraController->SetCurrentOffset(newOffset);

        const OriGine::Vec3f& currentTargetOffset = cameraController->GetCurrentTargetOffset();
        OriGine::Vec3f newTargetOffset            = Lerp<3, float>(currentTargetOffset, cameraTargetOffsetOnWallRun_, EaseOutCubic(t));
        cameraController->SetCurrentTargetOffset(newTargetOffset);
    }
}

void PlayerWallRunState::Finalize() {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntity);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntity);
    rigidbody->SetUseGravity(true); // 重力を有効

    transform->translate += wallNormal_ * 0.1f;

    transform->rotate = Quaternion::Identity();

    /// TODO: カメラの処理をここに書くべきではない
    // カメラの傾きを徐々に変える
    OriGine::Entity* gameCameraEntity = scene_->GetEntity(state->GetCameraEntityID());
    if (!gameCameraEntity) {
        return;
    }
    CameraController* cameraController = scene_->GetComponent<CameraController>(gameCameraEntity);
    if (cameraController) {
        cameraController->SetCurrentOffset(cameraController->GetOffsetOnDash());
        cameraController->SetCurrentTargetOffset(cameraController->GetTargetOffsetOnDash());
    }
}

PlayerMoveState PlayerWallRunState::TransitionState() const {
    auto* playerEntity = scene_->GetEntity(playerEntityID_);

    if (separationLeftTime_ <= 0.0f) {
        return PlayerMoveState::FALL_DOWN;
    }

    auto playerInput = scene_->GetComponent<PlayerInput>(playerEntity);
    if (playerInput->IsWallJumpInput()) {
        return PlayerMoveState::WALL_JUMP;
    }

    return PlayerMoveState::WALL_RUN;
}
