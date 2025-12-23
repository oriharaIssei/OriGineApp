#pragma once

/// stl
#include <concepts>
#include <memory>

/// ECS
// component
#include "IPlayerMoveState.h"
#include "PlayerState.h"

std::shared_ptr<IPlayerMoveState> CreatePlayerMoveStateByEnum(PlayerMoveState _state, OriGine::Scene* _scene, int32_t _playerEntityID);
