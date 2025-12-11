#include "IPlayerMoveState.h"

IPlayerMoveState::IPlayerMoveState(OriGine::Scene* _scene, int32_t _playerEntityID, PlayerMoveState _state) : scene_(_scene), playerEntityID_(_playerEntityID), state_(_state) {}
IPlayerMoveState::~IPlayerMoveState() {}
