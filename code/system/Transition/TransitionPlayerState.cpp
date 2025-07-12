#include "TransitionPlayerState.h"

/// engine
#define DELTA_TIME
#define ENGINE_ECS
#include "EngineInclude.h"
#include "sceneManager/SceneManager.h"

/// application
// component
#include "component/animation/SkinningAnimationComponent.h"
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
        auto* skinningAnimation = getComponent<SkinningAnimationComponent>(_entity);
        playerStatus->getPlayerMoveState()->Finalize();

        int32_t animationIndex = -1;
        switch (playerStatus->getState()) {
        case PlayerMoveState::IDLE: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerIdleState>(_entity->getID()));
            animationIndex = 0; // IDLE アニメーションのインデックス
            break;
        }
        case PlayerMoveState::DASH: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerDashState>(_entity->getID()));
            animationIndex = 1; // DASH アニメーションのインデックス
            break;
        }
        case PlayerMoveState::JUMP: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerJumpState>(_entity->getID()));
            animationIndex = 2;
            break;
        }
        case PlayerMoveState::FALL_DOWN: {
            playerStatus->setPlayerMoveState(std::make_shared<PlayerFallDownState>(_entity->getID()));
            animationIndex = 3; // FALL_DOWN アニメーションのインデックス
            break;
        }
        case PlayerMoveState::WALL_RUN: {
            animationIndex = 1;
            playerStatus->setPlayerMoveState(std::make_shared<PlayerWallRunState>(_entity->getID()));
            break;
        }
        case PlayerMoveState::WALL_JUMP: {
            animationIndex = 2;
            playerStatus->setPlayerMoveState(std::make_shared<PlayerWallJumpState>(_entity->getID()));
            break;
        }
        }

        // アニメーションのインデックスが有効な場合のみ、アニメーションを再生
        if (animationIndex >= 0 && animationIndex < skinningAnimation->getAnimationTable().size()) {
            skinningAnimation->endTransition(); // トランジションを終了
            skinningAnimation->PlayNext(animationIndex, 0.08f); // トランジションのブレンド時間を設定
        }

        playerStatus->getPlayerMoveState()->Initialize();
    }
}
