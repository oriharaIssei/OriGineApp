#pragma once

/// parent
#include "component/Player/State/IPlayerMoveState.h"

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
};
