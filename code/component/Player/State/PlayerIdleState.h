#pragma once

/// parent
#include "IPlayerMoveState.h"

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
};
