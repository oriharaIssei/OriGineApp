#pragma once
/// parent
#include "component/IComponent.h"

/// stl
#include <vector>

/// engine
#include "scene/Scene.h"
// lib
#include "input/Input.h"

/// component
#include "component/Player/State/PlayerState.h"

/// math
#include "math/Vector2.h"
#include "math/Vector3.h"

class IPlayerMoveState {
public:
    IPlayerMoveState(Scene* _scene, int32_t _playerEntityID, PlayerMoveState _state);
    virtual ~IPlayerMoveState();

    virtual void Initialize()             = 0;
    virtual void Update(float _deltaTime) = 0;
    virtual void Finalize()               = 0;

    virtual PlayerMoveState TransitionState() const = 0;

protected:
    Scene* scene_           = nullptr; // シーンへのポインタ
    int32_t playerEntityID_ = -1; // プレイヤーのエンティティID
private:
    // このクラスが表す移動状態
    PlayerMoveState state_ = PlayerMoveState::IDLE;

public:
    const PlayerMoveState& getState() const {
        return state_;
    }
    void setState(const PlayerMoveState& _state) {
        state_ = _state;
    }
};
