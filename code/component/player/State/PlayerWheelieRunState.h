#pragma once

#include "IPlayerMoveState.h"

/// Component
struct Transform;

/// Math
#include "Vector3.h"

/// <summary>
/// Playerの壁走り(垂直)状態
/// </summary>
class PlayerWheelieRunState
    : public IPlayerMoveState {
public:
    PlayerWheelieRunState(OriGine::Scene* _scene, OriGine::EntityHandle _playerEntityHandle) : IPlayerMoveState(_scene, _playerEntityHandle, PlayerMoveState::WHEELIE_RUN) {}
    ~PlayerWheelieRunState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;

    PlayerMoveState TransitionState() const override;

protected:
    OriGine::Vec3f wallNormal_ = OriGine::Vec3f(0.0f, 0.0f, 0.0f); // 壁の法線
    float separationGraceTime_ = 0.04f; // オブジェクトが離れていると判定するまでの猶予時間
    float separationLeftTime_  = 0.0f; // 壁との衝突判定の残り時間

    OriGine::Quaternion playerBeforeRotate_ = OriGine::Quaternion();
    OriGine::Quaternion playerRotateTarget_ = OriGine::Quaternion();
    const float kRotateTime_                = 0.36f;
    float rotateTimer_                      = 0.f;
};
