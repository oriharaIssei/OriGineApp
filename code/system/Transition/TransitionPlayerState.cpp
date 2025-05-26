#include "TransitionPlayerState.h"

/// engine
#define DELTA_TIME
#define ENGINE_ECS
#include "EngineInclude.h"

/// application
// component
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"

void TransitionPlayerState::UpdateEntity(GameEntity* _entity) {
    const float deltaTime      = getMainDeltaTime();
    PlayerInput* playerInput   = getComponent<PlayerInput>(_entity);
    PlayerStatus* playerStatus = getComponent<PlayerStatus>(_entity);

    /// =====================================================
    // StateUpdate
    /// =====================================================
    if (playerInput->getInputDirection().length() > 0) {
        playerStatus->setState(PlayerStatus::MoveState::DASH);
    } else {
        playerStatus->setState(PlayerStatus::MoveState::IDLE);
    }

    /// =====================================================
    // Gear
    /// =====================================================
    playerStatus->setGearUp(false);

    switch (playerStatus->getState()) {
    case PlayerStatus::MoveState::IDLE: {
        playerStatus->setGearUpCoolTime(1.f * playerStatus->getGearLevel());
        break;
    }
    case PlayerStatus::MoveState::DASH: {
        // gearLevel の更新
        playerStatus->minusGearUpCoolTime(deltaTime);
        if (playerStatus->getGearUpCoolTime() <= 0.f) {
            playerStatus->setGearUp(true);
            playerStatus->setGearLevel(playerStatus->getGearLevel() + 1);
            playerStatus->setGearUpCoolTime(1.f * playerStatus->getGearLevel());
        }
        break;
    }
    case PlayerStatus::MoveState::JUMP: {
        break;
    }
    case PlayerStatus::MoveState::SLIDE: {
        break;
    }
    default:
        break;
    }
}
