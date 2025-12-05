#pragma once

/// parent
#include "IPlayerMoveState.h"

/// <summary>
/// Playerの待機状態を表すクラス
/// </summary>
class PlayerIdleState
    : public IPlayerMoveState {
public:
    PlayerIdleState(Scene* _scene, int32_t _playerEntityID) : IPlayerMoveState(_scene, _playerEntityID, PlayerMoveState::IDLE) {}
    ~PlayerIdleState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;
    PlayerMoveState TransitionState() const override;

private:
    const float kCameraOffsetLerpTime_ = 0.7f;
    float cameraOffsetLerpTimer_       = 0.0f;
    const float kFallDownThresholdTime_ = 0.2f; // 落下状態に遷移するまでの閾値時間
    float fallDownTimer_               = 0.0f; // 落下時間計測用 
};
