#include "TransitionPlayerState.h"

/// engine
#define DELTA_TIME
#define ENGINE_ECS
#include "EngineInclude.h"
#include "scene/SceneManager.h"

/// application
// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/State/PlayerState.h"
#include "component/SceneChanger.h"

#include "component/Player/State/PlayerIdleState.h"
#include "component/Player/State/PlayerDashState.h"
#include "component/Player/State/PlayerJumpState.h"
#include "component/Player/State/PlayerFallDownState.h"
#include "component/Player/State/PlayerWallRunState.h"
#include "component/Player/State/PlayerWallJumpState.h"

#include "component/Camera/CameraController.h"
#include "component/transform/CameraTransform.h"

void TransitionPlayerState::UpdateEntity(GameEntity* _entity) {
    PlayerState* state = getComponent<PlayerState>(_entity);

    // 一フレームだけ trueになればいいので 毎フレーム 初期化
    state->setGearUp(false);

    if (state->isGoal()) {
        SceneChanger* sceneChanger = getComponent<SceneChanger>(_entity);
        if (sceneChanger) {
            sceneChanger->ChangeScene();
            return;
        }
    }

    /// =====================================================
    // PlayerMoveState の初期化 (必要なら)
    /// =====================================================
    if (!state->getPlayerMoveState()) {
        state->setPlayerMoveState(std::make_shared<PlayerIdleState>(this->getScene(), _entity->getID()));
        state->setState(PlayerMoveState::IDLE);
    }

    /// =====================================================
    // StateUpdate
    /// =====================================================
    state->setPrevState(state->getStateEnum());
    state->setState(state->getPlayerMoveState()->TransitionState());

    // 状態が変わった場合、状態を更新
    if (state->getStateEnum() != state->getPrevStateEnum()) {
        auto* skinningAnimation = getComponent<SkinningAnimationComponent>(_entity);
        state->getPlayerMoveState()->Finalize();

        int32_t animationIndex = -1;
        switch (state->getStateEnum()) {
        case PlayerMoveState::IDLE: {
            state->setPlayerMoveState(std::make_shared<PlayerIdleState>(this->getScene(), _entity->getID()));
            animationIndex = 0; // IDLE アニメーションのインデックス
            break;
        }
        case PlayerMoveState::DASH: {
            state->setPlayerMoveState(std::make_shared<PlayerDashState>(this->getScene(), _entity->getID()));
            animationIndex = 1; // DASH アニメーションのインデックス
            break;
        }
        case PlayerMoveState::JUMP: {
            state->setPlayerMoveState(std::make_shared<PlayerJumpState>(this->getScene(), _entity->getID()));
            animationIndex = 2;
            break;
        }
        case PlayerMoveState::FALL_DOWN: {
            state->setPlayerMoveState(std::make_shared<PlayerFallDownState>(this->getScene(), _entity->getID()));
            animationIndex = 3; // FALL_DOWN アニメーションのインデックス
            break;
        }
        case PlayerMoveState::WALL_RUN: {
            animationIndex = 1;
            state->setPlayerMoveState(std::make_shared<PlayerWallRunState>(this->getScene(), _entity->getID()));
            break;
        }
        case PlayerMoveState::WALL_JUMP: {
            animationIndex = 2;
            state->setPlayerMoveState(std::make_shared<PlayerWallJumpState>(this->getScene(), _entity->getID()));
            break;
        }
        }

        // アニメーションのインデックスが有効な場合のみ、アニメーションを再生
        if (animationIndex >= 0 && animationIndex < skinningAnimation->getAnimationTable().size()) {
            skinningAnimation->endTransition(); // トランジションを終了
            skinningAnimation->PlayNext(animationIndex, 0.08f); // トランジションのブレンド時間を設定
        }

        state->getPlayerMoveState()->Initialize();
    }

    /// =====================================================
    // Fov Y
    /// =====================================================
    GameEntity* gameCamera             = getUniqueEntity("GameCamera");
    CameraController* cameraController = getComponent<CameraController>(gameCamera);
    if (cameraController) {
        // fov 更新
        CameraTransform* cameraTransform = getComponent<CameraTransform>(gameCamera);
        if (cameraTransform) {
            cameraTransform->fovAngleY = std::lerp(cameraTransform->fovAngleY, cameraController->CalculateFovY(state->getGearLevel()), cameraController->getFovYInterpolate());
        };
    }
}
