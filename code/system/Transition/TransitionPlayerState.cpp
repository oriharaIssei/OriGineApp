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
    PlayerState* state = GetComponent<PlayerState>(_entity);

    // 一フレームだけ trueになればいいので 毎フレーム 初期化
    state->SetGearUp(false);

    if (state->IsGoal()) {
        SceneChanger* sceneChanger = GetComponent<SceneChanger>(_entity);
        if (sceneChanger) {
            sceneChanger->ChangeScene();
            return;
        }
    }

    /// =====================================================
    // PlayerMoveState の初期化 (必要なら)
    /// =====================================================
    if (!state->GetPlayerMoveState()) {
        state->SetPlayerMoveState(CreatePlayerMoveStateByEnum(PlayerMoveState::IDLE, this->GetScene(), _entity->GetID()));
    }

    /// =====================================================
    // StateUpdate
    /// =====================================================
    state->SetPrevState(state->GetStateEnum());
    PlayerMoveState newState = state->GetPlayerMoveState()->TransitionState();

    // 状態が変わった場合、状態を更新
    if (newState != state->GetPrevStateEnum()) {
        state->GetPlayerMoveState()->Finalize();

        state->SetPlayerMoveState(CreatePlayerMoveStateByEnum(newState, this->GetScene(), _entity->GetID()));
        state->GetPlayerMoveState()->Initialize();
    }

    /// =====================================================
    // Fov Y
    /// =====================================================
    Entity* gameCamera = GetUniqueEntity("GameCamera");
    if (!gameCamera) {
        return;
    }
    CameraController* cameraController = GetComponent<CameraController>(gameCamera);
    if (cameraController) {
        // fov 更新
        CameraTransform* cameraTransform = GetComponent<CameraTransform>(gameCamera);
        if (cameraTransform) {
            cameraTransform->fovAngleY = std::lerp(cameraTransform->fovAngleY, cameraController->CalculateFovYByPlayerGearLevel(state->GetGearLevel()), cameraController->GetFovYInterpolate());
        };
    }
}
