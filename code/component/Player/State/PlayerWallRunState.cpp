#include "PlayerWallRunState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/renderer/MeshRenderer.h"
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

namespace {
constexpr float kOffsetRate = 0.1f;
}

void PlayerWallRunState::Initialize() {
    constexpr int32_t thresholdGearLevel = 3;
    constexpr float kMeshOffsetRate      = 0.26f;

    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);

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
    rigidbody->SetVelocity(Y, playerStatus->GetUpwardForceOnWallRun());

    rigidbody->SetMass(playerStatus->GetMassOnWallRun());

    // 壁との分離猶予
    separationLeftTime_ = separationGraceTime_;

    // ===== 向きとロール =====
    PlayerEffectControlParam* effectParam =
        scene_->GetComponent<PlayerEffectControlParam>(playerEntityHandle_);

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
    CameraController* cameraController =
        scene_->GetComponent<CameraController>(state->GetCameraEntityHandle());

    cameraController->destinationAngleXY = Vec2f(0.f, 0.f);

    // 左壁想定のオフセットを取得
    cameraTargetOffsetOnWallRun_ =
        cameraController->targetOffsetOnWallRun;

    cameraOffsetOnWallRun_ =
        cameraController->offsetOnWallRun;

    // 左壁想定の回転角度を取得
    cameraRotateZOnWallRun_ = cameraController->rotateZOnWallRun;
    // ===== 右壁なら左右反転 =====
    if (!isRightWall) {
        cameraTargetOffsetOnWallRun_[X] *= -1.0f;
        cameraOffsetOnWallRun_[X] *= -1.0f;

        cameraRotateZOnWallRun_ *= -1.f;
    }

    cameraAngleLerpTimer_ = 0.0f;

    auto* modelRenderer = scene_->GetComponent<ModelMeshRenderer>(playerEntityHandle_);
    if (modelRenderer) {
        for (auto& mesh : modelRenderer->GetAllTransformBuffRef()) {
            mesh.openData_.translate -= wallNormal_ * kMeshOffsetRate;
        }
    }
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* state     = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* transform = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);

    // 衝突が途切れないようにめり込ませる
    transform->translate -= wallNormal_ * kOffsetRate;
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
    auto* rigidbody          = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    OriGine::Vec3f direction = rigidbody->GetVelocity().normalize();
    OriGine::Vec3f newVelo   = direction * (playerSpeed_ * currentSpeedRate);
    rigidbody->SetVelocity(newVelo);
    rigidbody->SetMaxXZSpeed(newVelo.length());

    /// TODO: カメラの処理をここに書くべきではない
    // カメラの傾きを徐々に変える
    cameraAngleLerpTimer_ += _deltaTime;
    float t = cameraAngleLerpTimer_ / kCameraAngleLerpTime_;
    t       = std::clamp(t, 0.f, 1.f);

    // 一度だけ実行
    if (t >= 1) {
        return;
    }

    CameraController* cameraController = scene_->GetComponent<CameraController>(state->GetCameraEntityHandle());
    if (cameraController) {
        cameraController->currentOffset       = Lerp<3, float>(cameraController->currentOffset, cameraOffsetOnWallRun_, EaseOutCubic(t));
        cameraController->currentTargetOffset = Lerp<3, float>(cameraController->currentTargetOffset, cameraTargetOffsetOnWallRun_, EaseOutCubic(t));

        cameraController->currentRotateZ = std::lerp(0.f, cameraRotateZOnWallRun_, EaseOutCubic(t));
    }
}

void PlayerWallRunState::Finalize() {
    auto* state        = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);

    rigidbody->SetMass(playerStatus->GetDefaultMass());
    playerStatus->ResetWallRunInterval();

    transform->translate += wallNormal_ * kOffsetRate;

    /// TODO: カメラの処理をここに書くべきではない
    CameraController* cameraController = scene_->GetComponent<CameraController>(state->GetCameraEntityHandle());
    if (cameraController) {
        // onDashに差し替える
        cameraController->currentOffset       = cameraController->offsetOnWallRun;
        cameraController->currentTargetOffset = cameraController->targetOffsetOnDash;
        cameraController->currentRotateZ      = 0.f;
    }

    auto* modelRenderer = scene_->GetComponent<ModelMeshRenderer>(playerEntityHandle_);
    if (modelRenderer) {
        for (auto& mesh : modelRenderer->GetAllTransformBuffRef()) {
            mesh.openData_.translate = Vec3f();
        }
    }
}

PlayerMoveState PlayerWallRunState::TransitionState() const {
    if (separationLeftTime_ <= 0.0f) {
        return PlayerMoveState::FALL_DOWN;
    }

    auto playerInput = scene_->GetComponent<PlayerInput>(playerEntityHandle_);
    if (playerInput->IsWallJumpInput()) {
        return PlayerMoveState::WALL_JUMP;
    }

    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    if (rigidbody->GetVelocity()[Y] < -playerStatus->GetUpwardForceOnWallRun() * 0.3f) {
        return PlayerMoveState::FALL_DOWN;
    }

    return PlayerMoveState::WALL_RUN;
}
