#pragma once

/// parent
#include "component/Player/State/IPlayerMoveState.h"

class PlayerJumpState
    : public IPlayerMoveState {
public:
    PlayerJumpState(Scene* _scene, int32_t _playerEntityID) : IPlayerMoveState(_scene, _playerEntityID, PlayerMoveState::JUMP) {}
    ~PlayerJumpState() override {};
    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;
    PlayerMoveState TransitionState() const override;

private:
    float releaseJumpPower_ = 0.0f; // ジャンプボタンを離した時のジャンプパワー
};
