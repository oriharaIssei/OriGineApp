#include "PlayerWallRunState.h"

/// engine
#include "scene/SceneFactory.h"

/// ECS
// component
#include "component/physics/Rigidbody.h"
#include "component/renderer/ModelMeshRenderer.h"
#include "component/spline/SplinePoints.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

#include "component/player/PlayerEffectControlParam.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerState.h"

#include "component/Camera/CameraController.h"

/// log
#include "logger/Logger.h"

/// util
#include "util/globalVariables/SerializedField.h"

/// math
#include "math/mathEnv.h"
#include "MyEasing.h"

#include "component/player/PlayerConfig.h"

using namespace OriGine;

namespace {
constexpr float kOffsetRate = AppConfig::Player::kWallRunOffsetRate;
}

void PlayerWallRunState::Initialize() {
    constexpr int32_t thresholdGearLevel = AppConfig::Player::kWallRunThresholdGearLevel;
    constexpr float kMeshOffsetRate      = AppConfig::Player::kWallRunMeshOffsetRate;

    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* state        = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);

    // 壁ジャンプ前の速度
    prevVelo_ = rigidbody->GetVelocity();

    // 壁法線
    wallNormal_ = state->GetWallCollisionNormal().normalize();

    // ===== 進行方向を「速度を壁面に投影して」求める =====
    OriGine::Vec3f direction = prevVelo_ - wallNormal_ * OriGine::Vec3f::Dot(prevVelo_, wallNormal_);
    direction[Y]             = 0.0f;

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

        playerSpeed_ = playerStatus->GetCurrentMaxSpeed();
    }

    // ===== 移動 =====
    rigidbody->SetVelocity(playerSpeed_ * direction);

    rigidbody->SetMass(playerStatus->GetMassOnWallRun());

    // 壁との分離猶予
    separationLeftTime_ = separationGraceTime_;
    // 壁走り離脱速度
    wallRunDetachSpeed_ = playerStatus->GetWallRunDetachSpeed();

    // ===== 向きとロール =====
    PlayerEffectControlParam* effectParam = scene_->GetComponent<PlayerEffectControlParam>(playerEntityHandle_);
    bool isRightWall                      = Vec3f::Dot(Vec3f::Cross(axisY, wallNormal_), direction) > 0.0f;

    float rotateZOffsetOnWallRun = effectParam->GetRotateOffsetOnWallRun();
    // プレイヤーの向きを移動方向に合わせる
    Quaternion lookForward = Quaternion::LookAt(direction, axisY);
    transform->rotate      = lookForward;
    // 回転アニメーションのゴール地点を設定
    Quaternion angleOffset = Quaternion::RotateAxisAngle(axisZ, isRightWall ? rotateZOffsetOnWallRun : -rotateZOffsetOnWallRun);
    playerBeforeRotate_    = lookForward;
    playerRotateTarget_    = lookForward * angleOffset;

    // ===== スピード制御 =====
    speedRate_        = playerStatus->GetWallRunRate();
    speedRumpUpTime_  = playerStatus->GetWallRunRampUpTime();
    speedRumpUpTimer_ = 0.0f;

    // ===== 重力制御 =====
    gravityApplyDelay_ = playerStatus->GetGravityApplyDelayOnWallRun();
    rigidbody->SetUseGravity(false);

    // ===== メッシュのオフセット =====
    auto* modelRenderer = scene_->GetComponent<ModelMeshRenderer>(playerEntityHandle_);
    if (modelRenderer) {
        for (auto& mesh : modelRenderer->GetAllTransformBuffRef()) {
            mesh.openData_.translate -= isRightWall ? -wallNormal_ * kMeshOffsetRate : wallNormal_ * kMeshOffsetRate;
        }
    }

    // ===== カメラ =====
    CameraController* cameraController = scene_->GetComponent<CameraController>(state->GetCameraEntityHandle());

    // 左壁想定のオフセットを取得
    cameraTargetOffsetOnWallRun_ = cameraController->targetOffsetOnWallRun;

    cameraOffsetOnWallRun_ = cameraController->offsetOnWallRun;

    // 左壁想定の回転角度を取得
    cameraRotateZOnWallRun_ = cameraController->rotateZOnWallRun;
    // ===== 右壁なら左右反転 =====
    if (!isRightWall) {
        cameraTargetOffsetOnWallRun_[X] *= -1.0f;
        cameraOffsetOnWallRun_[X] *= -1.0f;

        cameraRotateZOnWallRun_ *= -1.f;
    }

    cameraAngleLerpTimer_ = 0.0f;
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* state     = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* transform = scene_->GetComponent<OriGine::Transform>(playerEntityHandle_);

    // 衝突が途切れないようにめり込ませる
    transform->translate -= wallNormal_ * kOffsetRate;

    // 回転アニメーション
    rotateTimer_ += _deltaTime;
    float rotateT     = (std::min)(rotateTimer_ / kRotateTime_, 1.f);
    transform->rotate = Slerp(playerBeforeRotate_, playerRotateTarget_, EaseOutQuad(rotateT));

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

    gravityApplyDelay_ -= _deltaTime;
    if (gravityApplyDelay_ <= 0.0f) {
        rigidbody->SetUseGravity(true);
        gravityApplyDelay_ = 0.f;
    }

    /// TODO: カメラの処理をここに書くべきではない
    // カメラの傾きを徐々に変える
    float cameraDeltaTime = Engine::GetInstance()->GetDeltaTimer()->GetScaledDeltaTime("Camera");
    cameraAngleLerpTimer_ += cameraDeltaTime;
    float t = cameraAngleLerpTimer_ / kCameraAngleLerpTime_;
    t       = std::clamp(t, 0.f, 1.f);

    // 一度だけ実行
    CameraController* cameraController = scene_->GetComponent<CameraController>(state->GetCameraEntityHandle());
    if (!cameraController) {
        return;
    }
    if (t >= 1) {
        cameraController->currentOffset       = cameraOffsetOnWallRun_;
        cameraController->currentTargetOffset = cameraTargetOffsetOnWallRun_;

        cameraController->currentRotateZ = cameraRotateZOnWallRun_;
        return;
    }

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
    rigidbody->SetUseGravity(true);
    playerStatus->SetupWallRunInterval();

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

    scene_->AddDeleteEntity(pathEntityHandle_);
}

PlayerMoveState PlayerWallRunState::TransitionState() const {
    if (separationLeftTime_ <= 0.0f) {
        return PlayerMoveState::FALL_DOWN;
    }

    auto playerInput = scene_->GetComponent<PlayerInput>(playerEntityHandle_);
    if (playerInput->IsWallJumpInput()) {
        return PlayerMoveState::WALL_JUMP;
    }

    auto* rigidbody = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    if (rigidbody->GetVelocity()[Y] < wallRunDetachSpeed_) {
        return PlayerMoveState::FALL_DOWN;
    }

    return PlayerMoveState::WALL_RUN;
}

void PlayerWallRunState::CreateWallRunPathEntity(const Vec3f& _origine, Rigidbody* _rigidbody, const OriGine::Vec3f& _direction) {
    SceneFactory factory;
    Entity* pathEntity = factory.BuildEntityFromTemplate(scene_, "WallRunPath");
    if (!pathEntity) {
        LOG_ERROR("Failed to create WallRunPath entity.");
        return;
    }

    auto* transform      = scene_->GetComponent<OriGine::Transform>(pathEntity->GetHandle());
    transform->translate = _origine;
    auto* splinePoints   = scene_->GetComponent<SplinePoints>(pathEntity->GetHandle());
    if (!splinePoints) {
        LOG_ERROR("WallRunPath entity does not have SplinePoints component.");
        return;
    }

    pathEntityHandle_ = pathEntity->GetHandle();

    SerializedField<float> gravity = SerializedField<float>("Settings", "Physics", "Gravity");
    float deltaTime                = Engine::GetInstance()->GetDeltaTimer()->GetScaledDeltaTime("Player");
    splinePoints->points           = SplinePointsSetup(_rigidbody, _direction, *gravity.GetValue(), deltaTime);
}

std::deque<Vec3f> PlayerWallRunState::SplinePointsSetup(Rigidbody* _rigidbody, const OriGine::Vec3f& _direction, float _gravity, float _deltaTime) {
    std::deque<Vec3f> splinePoints;

    float forwardSpeed = playerSpeed_;
    float upwardSpeed  = _rigidbody->GetVelocity()[Y];
    float accelY       = _rigidbody->GetAcceleration()[Y];
    float downSpeed    = (std::min)(_gravity * _rigidbody->GetMass(), _rigidbody->MaxFallSpeed());

    Vec3f pos = Vec3f();
    splinePoints.push_back(pos);
    while (true) {

        speedRumpUpTimer_ += _deltaTime;
        float rumpUpT          = speedRumpUpTimer_ / speedRumpUpTime_;
        rumpUpT                = std::clamp(rumpUpT, 0.f, 1.f);
        float currentSpeedRate = std::lerp(1.f, speedRate_, EaseOutCubic(rumpUpT));
        // 壁ジャンプ失敗と判定する速度閾値を下回ったら抜ける
        if (upwardSpeed < thresholdFailedWallJumpSpeed_) {
            break;
        }

        // 位置計算
        pos += _direction * (forwardSpeed * currentSpeedRate) * _deltaTime;

        accelY -= downSpeed;
        upwardSpeed += accelY * _deltaTime;
        pos[Y] += upwardSpeed * _deltaTime;

        splinePoints.push_back(pos);
    }

    return splinePoints;
}
