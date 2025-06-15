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
    playerStatus->setPrevState(playerStatus->getState());
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
        playerStatus->setGearUpCoolTime(playerStatus->getBaseGearupCoolTime());
        playerStatus->setCurrentSpeed(0.f);
        break;
    }
    case PlayerStatus::MoveState::DASH: {
        if (playerStatus->getPrevState() == PlayerStatus::MoveState::IDLE) {
            // 前の状態が IDLE の場合、ベーススピードを設定
            playerStatus->setCurrentSpeed(playerStatus->getBaseSpeed());
        }
        // gearLevel の更新
        playerStatus->minusGearUpCoolTime(deltaTime);
        if (playerStatus->getGearUpCoolTime() <= 0.f) {
            playerStatus->setGearUp(true);

            playerStatus->setGearLevel(playerStatus->getGearLevel() + 1);
            playerStatus->setGearUpCoolTime(playerStatus->getBaseGearupCoolTime() + (playerStatus->getGearLevel() * 1.3f));
            playerStatus->setCurrentSpeed(playerStatus->getBaseSpeed() + playerStatus->getBaseSpeed() * (playerStatus->getGearLevel() * 1.6f));
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
