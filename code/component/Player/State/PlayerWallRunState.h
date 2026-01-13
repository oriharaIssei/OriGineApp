
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
    PlayerWallRunState(OriGine::Scene* _scene, OriGine::EntityHandle _playerEntityHandle) : IPlayerMoveState(_scene, _playerEntityHandle, PlayerMoveState::WALL_RUN) {}
    ~PlayerWallRunState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;

    PlayerMoveState TransitionState() const override;

protected:
    OriGine::Vec3f prevVelo_   = OriGine::Vec3f(0.0f, 0.0f, 0.0f); // 前の速度 壁走り前の速度を保存
    float separationGraceTime_ = 0.04f; // オブジェクトが離れていると判定するまでの猶予時間
    float separationLeftTime_  = 0.0f; // 壁との衝突判定の残り時間

    float playerSpeed_      = 0.f; // playerの速度
    float speedRate_        = 1.f; // 壁走り速度倍率
    float speedRumpUpTime_  = 0.f; // 壁走り速度上昇時間
    float speedRumpUpTimer_ = 0.0f;

    OriGine::Vec3f wallNormal_ = OriGine::Vec3f(0.0f, 0.0f, 0.0f); // 壁の法線

    OriGine::Vec3f cameraTargetOffsetOnWallRun_ = OriGine::Vec3f(0.0f, 0.0f, 0.0f);
    OriGine::Vec3f cameraOffsetOnWallRun_       = OriGine::Vec3f(0.0f, 0.0f, 0.0f);

    float cameraRotateZOnWallRun_ = 0.f; // カメラのZ回転角度
    float currentCameraRotateZ_ = 0.f; // カメラのZ回転角度

    const float kCameraAngleLerpTime_ = 0.8f;
    float cameraAngleLerpTimer_       = 0.0f;
};
