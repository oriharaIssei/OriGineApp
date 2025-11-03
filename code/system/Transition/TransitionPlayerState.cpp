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
#include "component/Player/State/PlayerMoveStateFactory.h"
#include "component/Player/State/PlayerState.h"
#include "component/SceneChanger.h"

#include "component/Camera/CameraController.h"
#include "component/transform/CameraTransform.h"

void TransitionPlayerState::UpdateEntity(Entity* _entity) {
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
        state->setPlayerMoveState(CreatePlayerMoveStateByEnum(PlayerMoveState::IDLE, this->getScene(), _entity->getID()));
    }

    /// =====================================================
    // StateUpdate
    /// =====================================================
    state->setPrevState(state->getStateEnum());
    PlayerMoveState newState = state->getPlayerMoveState()->TransitionState();

    // 状態が変わった場合、状態を更新
    if (newState != state->getPrevStateEnum()) {
        state->getPlayerMoveState()->Finalize();

        state->setPlayerMoveState(CreatePlayerMoveStateByEnum(newState, this->getScene(), _entity->getID()));
        state->getPlayerMoveState()->Initialize();
    }

    /// =====================================================
    // Fov Y
    /// =====================================================
    Entity* gameCamera = getUniqueEntity("GameCamera");
    if (!gameCamera) {
        return;
    }
    CameraController* cameraController = getComponent<CameraController>(gameCamera);
    if (cameraController) {
        // fov 更新
        CameraTransform* cameraTransform = getComponent<CameraTransform>(gameCamera);
        if (cameraTransform) {
            cameraTransform->fovAngleY = std::lerp(cameraTransform->fovAngleY, cameraController->CalculateFovYByPlayerGearLevel(state->getGearLevel()), cameraController->getFovYInterpolate());
        };
    }
}
