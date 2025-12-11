#include "PlayerMoveStateFactory.h"

#include "PlayerDashState.h"
#include "PlayerFallDownState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerWallJumpState.h"
#include "PlayerWallRunState.h"

std::shared_ptr<IPlayerMoveState> CreatePlayerMoveStateByEnum(PlayerMoveState _state, OriGine::Scene* _scene, int32_t _playerEntityID) {
    switch (_state) {
    case PlayerMoveState::IDLE:
        return std::make_shared<PlayerIdleState>(_scene, _playerEntityID);
        break;
    case PlayerMoveState::DASH:
        return std::make_shared<PlayerDashState>(_scene, _playerEntityID);
        break;
    case PlayerMoveState::FALL_DOWN:
        return std::make_shared<PlayerFallDownState>(_scene, _playerEntityID);
        break;
    case PlayerMoveState::JUMP:
        return std::make_shared<PlayerJumpState>(_scene, _playerEntityID);
        break;
    case PlayerMoveState::WALL_RUN:
        return std::make_shared<PlayerWallRunState>(_scene, _playerEntityID);
        break;
    case PlayerMoveState::WALL_JUMP:
        return std::make_shared<PlayerWallJumpState>(_scene, _playerEntityID);
        break;
    default:
        return nullptr;
        break;
    }
}
