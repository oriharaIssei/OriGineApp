#pragma once

#include "IPlayerMoveState.h"

/// Component
struct Transform;

/// Math
#include "Matrix4x4.h"

/// <summary>
/// Playerの壁走り状態
/// </summary>
class PlayerWallRunState
    : public IPlayerMoveState {
public:
    PlayerWallRunState(Scene* _scene, int32_t _playerEntityID) : IPlayerMoveState(_scene, _playerEntityID, PlayerMoveState::WALL_RUN) {}
    ~PlayerWallRunState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;

    PlayerMoveState TransitionState() const override;

protected:
    Vec3f prevVelo_            = Vec3f(0.0f, 0.0f, 0.0f); // 前の速度 壁走り前の速度を保存
    float separationGraceTime_ = 0.04f; // オブジェクトが離れていると判定するまでの猶予時間
    float separationLeftTime_  = 0.0f; // 壁との衝突判定の残り時間

    Vec3f wallNormal_ = Vec3f(0.0f, 0.0f, 0.0f); // 壁の法線

    Vec3f cameraTargetOffsetOnWallRun_ = Vec3f(0.0f, 0.0f, 0.0f);
    Vec3f cameraOffsetOnWallRun_       = Vec3f(0.0f, 0.0f, 0.0f);

    const float kCameraAngleLerpTime_ = 0.8f;
    float cameraAngleLerpTimer_       = 0.0f;
};
