#pragma once

/// parent
#include "component/Player/State/IPlayerMoveState.h"

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
    const float kCameraOffsetLerpTime_               = 1.3f;
    float cameraOffsetLerpTimer_                     = 0.0f;
};
