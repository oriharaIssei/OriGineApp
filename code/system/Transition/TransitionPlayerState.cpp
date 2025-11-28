#include "TransitionPlayerState.h"

/// engine
#define DELTA_TIME
#define ENGINE_ECS
#include "EngineInclude.h"
#include "scene/SceneManager.h"

/// application
#include "manager/PlayerProgressStore.h"

// component
#include "component/SceneChanger.h"
#include "component/TimerComponent.h"

#include "component/material/Material.h"

#include "component/stage/StageData.h"

#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerMoveStateFactory.h"
#include "component/player/state/PlayerState.h"

#include "component/physics/Rigidbody.h"

#include "component/Camera/CameraController.h"
#include "component/transform/CameraTransform.h"

/// math
#include "math/MyEasing.h"

void TransitionPlayerState::UpdateEntity(Entity* _entity) {
    PlayerState* state = GetComponent<PlayerState>(_entity);

    /// =====================================================
    // StateUpdate
    /// =====================================================
    // goal 判定
    ///! TODO : 専用のシステムクラスで判定
    if (state->IsGoal()) {
        // clearTime Set
        PlayerProgressStore* progressStore = PlayerProgressStore::GetInstance();
        Entity* timerEntity                = GetUniqueEntity("Timer");
        Entity* stageDataEntity            = GetUniqueEntity("StageData");
        if (timerEntity && stageDataEntity) {
            StageData* stageData  = GetComponent<StageData>(stageDataEntity);
            TimerComponent* timer = GetComponent<TimerComponent>(timerEntity);
            if (timer && stageData) {
                progressStore->StageCleared(stageData->GetStageNumber(), stageData->GetDifficulty(), timer->GetTime());
            }
        }

        // sceneChange !
        SceneChanger* sceneChanger = GetComponent<SceneChanger>(_entity);
        if (sceneChanger) {
            sceneChanger->ChangeScene();
            return;
        }
    }

    // Restart 判定
    if (state->IsRestart()) {
        Transform* playerTransform = GetComponent<Transform>(_entity);
        playerTransform->Initialize(_entity);

        Entity* startPosEntity = GetUniqueEntity("StartPosition");
        if (startPosEntity) {
            Transform* startTransform = GetComponent<Transform>(startPosEntity);
            if (startTransform && playerTransform) {
                playerTransform->translate = startTransform->GetWorldTranslate();
            }
        };
        PlayerStatus* playerStatus = GetComponent<PlayerStatus>(_entity);
        Rigidbody* rigidbody       = GetComponent<Rigidbody>(_entity);

        // 初期化
        state->Initialize(_entity);
        playerStatus->Initialize(_entity);
        rigidbody->Initialize(_entity);
    }

    /// =====================================================
    // PlayerMoveState の初期化 (必要なら)
    /// =====================================================
    if (!state->GetPlayerMoveState()) {
        state->GetStateEnumRef().Set(PlayerMoveState::IDLE);
        state->SetPlayerMoveState(CreatePlayerMoveStateByEnum(PlayerMoveState::IDLE, this->GetScene(), _entity->GetID()));
    }

    /// =====================================================
    // MoveStateUpdate
    /// =====================================================
    auto& moveStateFlag = state->GetStateEnumRef();
    moveStateFlag.Set(state->GetPlayerMoveState()->TransitionState());

    // 状態が変わった場合、状態を更新
    if (moveStateFlag.IsChanged()) {
        state->GetPlayerMoveState()->Finalize();

        state->SetPlayerMoveState(CreatePlayerMoveStateByEnum(moveStateFlag.Current().ToEnum(), this->GetScene(), _entity->GetID()));
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

    // state Update
    EnumBitmask<PlayerStateFlag> newFlag = state->GetStateFlagRef().Current();
    newFlag.ClearFlag(PlayerStateFlag::GEAR_UP);
    newFlag.ClearFlag(PlayerStateFlag::IS_GOAL);
    newFlag.ClearFlag(PlayerStateFlag::IS_PENALTY);
    newFlag.ClearFlag(PlayerStateFlag::IS_RESTART);

    state->GetStateFlagRef().Set(newFlag);

    ///! Penalty専用のエフェクトシステムを作る
    // ペナルティ時間 更新
    state->SubtractPenaltyTime(GetMainDeltaTime());
    float currentInvisibleTime = state->GetInvincibilityTime();
    if (currentInvisibleTime > 0.f) {
        constexpr float kAmplitude = 13.f;
        state->SubtractInvincibilityTime(GetMainDeltaTime());

        PlayerStatus* playerStatus = GetComponent<PlayerStatus>(_entity);

        // animation 更新
        Material* material = GetComponent<Material>(_entity);
        float t            = currentInvisibleTime / playerStatus->GetInvincibilityTime();
        // 0~1 の間で 点滅させる
        material->color_[A] = std::clamp(EaseInSine((sinf(t * kAmplitude) * 0.5f) + 0.5f), 0.f, 1.f);
    } else {
        Material* material  = GetComponent<Material>(_entity);
        material->color_[A] = 1.f;
    }
}
