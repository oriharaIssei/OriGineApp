#include "PlayerOnRailState.h"

/// engine
#include "scene/Scene.h"

/// ECS
// component
#include "component/physics/Rigidbody.h"
#include "component/player/PlayerInput.h"
#include "component/player/PlayerStatus.h"
#include "component/player/State/PlayerState.h"
#include "component/gimmick/RailPoints.h"
#include "component/transform/Transform.h"

/// math
#include "math/mathEnv.h"
#include "math/MyEasing.h"
#include "math/Spline.h"

using namespace OriGine;

PlayerOnRailState::PlayerOnRailState(OriGine::Scene* _scene, OriGine::EntityHandle _playerEntityHandle)
    : IPlayerMoveState(_scene, _playerEntityHandle, PlayerMoveState::ON_RAIL) {}
PlayerOnRailState::~PlayerOnRailState() {}

void PlayerOnRailState::Initialize() {
    auto* playerState  = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* railPoints   = scene_->GetComponent<RailPoints>(playerState->GetRailEntityHandle());
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<Transform>(playerEntityHandle_);

    // 移動は全て こちらで行うため、更新されないように速度情報を抜く
    rigidbody->SetUseGravity(false);
    rigidbody->SetVelocity(Vec3f());
    rigidbody->SetAcceleration(Vec3f());
    baseSpeed_ = rigidbody->GetMaxXZSpeed();
    rigidbody->SetMaxXZSpeed(baseSpeed_ * playerStatus->GetRailSpeedRate());
    defaultMaxFallDownSpeed_ = rigidbody->MaxFallSpeed();
    rigidbody->SetMaxFallSpeed(baseSpeed_ * playerStatus->GetRailSpeedRate());

    // 事前に必要な情報を計算する
    std::pair<uint32_t, uint32_t> firstSegmentPoint = CalcPointSegmentIndex(railPoints->points, transform->GetWorldTranslate());
    currentRailPointIndex_                          = firstSegmentPoint.first;
    nextRailPointIndex_                             = firstSegmentPoint.second;

    railTotalLength_ = CalcSplineLength(railPoints->points);
}

void PlayerOnRailState::Update(float _deltaTime) {
    auto* playerState  = scene_->GetComponent<PlayerState>(playerEntityHandle_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<Transform>(playerEntityHandle_);
    auto* railPoints   = scene_->GetComponent<RailPoints>(playerState->GetRailEntityHandle());

    // railがなければ
    if (!railPoints) {
        isOutOfRail_ = false;
        return;
    }

    if (currentRailPointIndex_ > railPoints->points.size()) {
        isOutOfRail_ = false;
        return;
    }

    // 使用する一時変数
    const Vec3f& currentPoint = railPoints->points[currentRailPointIndex_];
    const Vec3f& nextPoint    = railPoints->points[nextRailPointIndex_];
    currentDirection_         = nextPoint - currentPoint;
    Vec3f front               = currentDirection_.normalize();

    // 速度
    rampUpTimer_ += _deltaTime;
    float t       = (std::min)(EaseOutCubic(rampUpTimer_ / rampUpTime_), 1.f);
    currentSpeed_ = baseSpeed_ * std::lerp(1.f, playerStatus->GetRailSpeedRate(), t);

    // 現在地の更新
    traveledDistance_ += currentSpeed_ * _deltaTime;
    traveledDistance_                               = (std::min)(railTotalLength_, traveledDistance_);
    std::pair<uint32_t, uint32_t> movedSegmentPoint = CalcDistanceSegmentIndex(railPoints->points, traveledDistance_);
    currentRailPointIndex_                          = movedSegmentPoint.first;
    nextRailPointIndex_                             = movedSegmentPoint.second;

    // transform の更新
    transform->translate 
    transform->rotate = Quaternion::LookAt(front, axisY);
}

void PlayerOnRailState::Finalize() {
    auto* playerInput  = scene_->GetComponent<PlayerInput>(playerEntityHandle_);
    auto* playerStatus = scene_->GetComponent<PlayerStatus>(playerEntityHandle_);
    auto* transform    = scene_->GetComponent<Transform>(playerEntityHandle_);
    auto* rigidbody    = scene_->GetComponent<Rigidbody>(playerEntityHandle_);

    rigidbody->SetUseGravity(true);
    rigidbody->SetMaxFallSpeed(defaultMaxFallDownSpeed_);

    float inputX            = playerInput->GetInputDirection()[X];
    const Vec3f& jumpOffset = playerStatus->GetRailJumpOffset();
    Vec3f jumpVelo          = jumpOffset * currentSpeed_;

    // -1 ~ 1 を3等分 し、
    // -1$ ～ -0.333... の範囲なら 補正を左に
    // -0.333... ～ 0.333...の範囲なら 補正を正面に
    // 0.333... ～ 1 の範囲なら 補正を右に(通常状態)
    if (inputX > -1.f && inputX < -0.3333f) {
        jumpVelo[X] *= -1.f;
    } else if (inputX > -0.333f && inputX < 0.3333f) {
        jumpVelo += Vec3f(0.f, 0.5f, 0.5f) * jumpVelo[X];
        jumpVelo[X] = 0.f;
    }

    jumpVelo = transform->rotate.RotateVector(jumpVelo);
    rigidbody->SetVelocity(jumpVelo);
}

PlayerMoveState PlayerOnRailState::TransitionState() const {
    auto* playerInput = scene_->GetComponent<PlayerInput>(playerEntityHandle_);

    if (playerInput->IsRailJumpInput()) {
        return PlayerMoveState::FALL_DOWN;
    }

    if (traveledDistance_ >= railTotalLength_ - kEpsilon) {
        return PlayerMoveState::FALL_DOWN;
    }

    return this->GetState();
}
