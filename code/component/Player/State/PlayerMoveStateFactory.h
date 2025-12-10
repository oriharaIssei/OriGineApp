#pragma once

#include <concepts>
#include <memory>

#include "IPlayerMoveState.h"

std::shared_ptr<IPlayerMoveState> CreatePlayerMoveStateByEnum(PlayerMoveState _state,OriGine::Scene* _scene, int32_t _playerEntityID);
