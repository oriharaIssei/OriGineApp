#pragma once

/// parent
#include "component/Player/State/IPlayerMoveState.h"

/// <summary>
/// Playerの落下状態 (ジャンプではない)
/// </summary>
class PlayerFallDownState
    : public IPlayerMoveState {
public:
    PlayerFallDownState(Scene* _scene, int32_t _playerEntityID) : IPlayerMoveState(_scene, _playerEntityID, PlayerMoveState::FALL_DOWN) {}
    ~PlayerFallDownState() override {};
    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;
    PlayerMoveState TransitionState() const override;

private:
    const int32_t kThresholdGearLevelOfCameraOffset_ = 2;
    const float kCameraOffsetLerpTime_               = 2.3f;
    float cameraOffsetLerpTimer_                     = 0.0f;
};
