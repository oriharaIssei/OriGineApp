#pragma once

/// parent
#include "component/player/state/IPlayerMoveState.h"

/// <summary>
/// Playerのダッシュ状態クラス(基本移動)
/// </summary>
class PlayerDashState
    : public IPlayerMoveState {
public:
    PlayerDashState(Scene* _scene, int32_t _playerEntityID) : IPlayerMoveState(_scene, _playerEntityID, PlayerMoveState::DASH) {}
    ~PlayerDashState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;
    PlayerMoveState TransitionState() const override;

private:
    const int32_t kThresholdGearLevelOfCameraOffset_ = 2;
    const float kCameraOffsetLerpTime_               = 2.3f;
    float cameraOffsetLerpTimer_                     = 0.0f;
};
