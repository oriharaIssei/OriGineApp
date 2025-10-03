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
};
