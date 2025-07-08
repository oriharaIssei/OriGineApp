#include "TransitionPlayerState.h"

/// engine
#define DELTA_TIME
#define ENGINE_ECS
#include "EngineInclude.h"
#include "sceneManager/SceneManager.h"

/// application
// component
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/SceneChanger.h"

void TransitionPlayerState::UpdateEntity(GameEntity* _entity) {
    PlayerStatus* playerStatus = getComponent<PlayerStatus>(_entity);

    // 一フレームだけ trueになればいいので 毎フレーム 初期化
    playerStatus->setGearUp(false);

    if (playerStatus->isGoal()) {
        SceneChanger* sceneChanger = getComponent<SceneChanger>(_entity);
        if (sceneChanger) {
            SceneManager::getInstance()->changeScene(sceneChanger->getNextSceneName());
            return;
        }
    }

    /// =====================================================
    // StateUpdate
    /// =====================================================
    playerStatus->setPrevState(playerStatus->getState());
    playerStatus->setState(playerStatus->getPlayerMoveState()->TransitionState());

    // 状態が変わった場合、状態を更新
    if (playerStatus->getState() != playerStatus->getPrevState()) {
        playerStatus->getPlayerMoveState()->Finalize();

        switch (playerStatus->getState()) {
        case PlayerMoveState::IDLE: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerIdleState>(_entity->getID()));
            break;
        }
        case PlayerMoveState::DASH: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerDashState>(_entity->getID()));
            break;
        }
        case PlayerMoveState::FALL_DOWN: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerFallDownState>(_entity->getID()));
            break;
        }
        case PlayerMoveState::JUMP: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerJumpState>(_entity->getID()));
            break;
        }
        case PlayerMoveState::WALL_RUN: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerWallRunState>(_entity->getID()));
            break;
        }
        case PlayerMoveState::WALL_JUMP: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerWallJumpState>(_entity->getID()));
            break;
        }
        }

        playerStatus->getPlayerMoveState()->Initialize();
    }
}
