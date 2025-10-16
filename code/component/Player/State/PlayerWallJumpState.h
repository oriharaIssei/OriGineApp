#pragma once

#include "IPlayerMoveState.h"

#include "component/physics/Rigidbody.h"
#include "component/transform/Transform.h"

class StageWall;

class PlayerWallJumpState
    : public IPlayerMoveState {
public:
    PlayerWallJumpState(Scene* _scene, int32_t _playerEntityID) : IPlayerMoveState(_scene, _playerEntityID, PlayerMoveState::WALL_JUMP) {}
    ~PlayerWallJumpState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;

    PlayerMoveState TransitionState() const override;

protected:
    Vec3f velo_ = Vec3f(0.0f, 0.0f, 0.0f); // 壁ジャンプの方向
};
