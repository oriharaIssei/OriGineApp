#include "TransitionPlayerState.h"

/// engine
#define DELTA_TIME
#define ENGINE_ECS
#include "EngineInclude.h"

#include "scene/SceneFactory.h"
#include "scene/SceneManager.h"

/// application
#include "manager/PlayerProgressStore.h"

// component
#include "component/SceneChanger.h"
#include "component/TimerComponent.h"

#include "component/spline/TireSplinePoints.h"

#include "component/material/Material.h"

#include "component/stage/StageData.h"

#include "component/player/PlayerEffectControlParam.h"
#include "component/player/PlayerStatus.h"
#include "component/player/state/PlayerMoveStateFactory.h"
#include "component/player/state/PlayerState.h"

#include "component/Camera/CameraController.h"
#include "component/transform/CameraTransform.h"

#include "component/ghost/PlayRecordeComponent.h"

/// math
#include "math/mathEnv.h"
#include "math/MyEasing.h"

using namespace OriGine;

void TransitionPlayerState::UpdateEntity(EntityHandle _handle) {
    PlayerState* state = GetComponent<PlayerState>(_handle);

    /// =====================================================
    // StateUpdate
    /// =====================================================
    // goal 判定
    ///! TODO : 専用のシステムクラスで判定
    if (state->IsGoal()) {
        // clearTime Set
        PlayerProgressStore* progressStore = PlayerProgressStore::GetInstance();
        EntityHandle timerEntityHandle     = GetUniqueEntity("Timer");
        EntityHandle stageDataEntityHandle = GetUniqueEntity("StageData");
        if (timerEntityHandle.IsValid() && stageDataEntityHandle.IsValid()) {
            StageData* stageData  = GetComponent<StageData>(timerEntityHandle);
            TimerComponent* timer = GetComponent<TimerComponent>(stageDataEntityHandle);

            EntityHandle recorderEntityHandle  = GetUniqueEntity("Recorder");
            PlayRecordeComponent* playRecorder = GetComponent<PlayRecordeComponent>(recorderEntityHandle);

            if (timer != nullptr && stageData != nullptr) {
                progressStore->StageCleared(playRecorder != nullptr ? playRecorder->replayRecorder_.get() : nullptr, stageData->GetStageNumber(), stageData->GetDifficulty(), timer->GetTime());
            }
        }

        // sceneChange !
        SceneChanger* sceneChanger = GetComponent<SceneChanger>(_handle);
        if (sceneChanger) {
            sceneChanger->ChangeScene();
            return;
        }
    }

    /// =====================================================
    // PlayerMoveState の初期化 (必要なら)
    /// =====================================================
    if (!state->GetPlayerMoveState()) {
        state->GetStateEnumRef().Set(PlayerMoveState::IDLE);
        state->SetPlayerMoveState(CreatePlayerMoveStateByEnum(PlayerMoveState::IDLE, this->GetScene(), _handle));
    }

    /// =====================================================
    // MoveStateUpdate
    /// =====================================================
    auto& moveStateFlag = state->GetStateEnumRef();
    moveStateFlag.Set(state->GetPlayerMoveState()->TransitionState());

    // 状態が変わった場合、状態を更新
    if (moveStateFlag.IsChanged()) {
        state->GetPlayerMoveState()->Finalize();

        state->SetPlayerMoveState(CreatePlayerMoveStateByEnum(moveStateFlag.Current().ToEnum(), this->GetScene(), _handle));
        state->GetPlayerMoveState()->Initialize();
    }

    PlayerEffectControlParam* effectParam = GetComponent<PlayerEffectControlParam>(_handle);
    if (effectParam && effectParam->GetTireTrailSplineEntityHandle().IsValid()) {
        if (moveStateFlag.Current().ToEnum() == PlayerMoveState::DASH) {
            // タイヤトレイルを作る
            constexpr char kPlayerTireTrailEntityName[] = "PlayerTireTrail";
            SceneFactory sceneFactory;
            Entity* tierTrail = sceneFactory.BuildEntityFromTemplate(this->GetScene(), kPlayerTireTrailEntityName);
            if (tierTrail) {
                auto tireSplinePoints = GetComponent<TireSplinePoints>(tierTrail->GetHandle());
                if (tireSplinePoints) {
                    tireSplinePoints->commonSettings.playerEntityHandle = _handle;
                    effectParam->SetTireTrailSplineEntityId(tierTrail->GetHandle());
                }
            }
        }
    }

    /// =====================================================
    // Fov Y
    /// =====================================================
    OriGine::Entity* gameCamera = GetEntity(state->GetCameraEntityHandle());
    if (!gameCamera) {
        return;
    }
    CameraController* cameraController = GetComponent<CameraController>(gameCamera->GetHandle());
    if (cameraController) {
        // fov 更新
        CameraTransform* cameraTransform = GetComponent<CameraTransform>(gameCamera->GetHandle());
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

        PlayerStatus* playerStatus = GetComponent<PlayerStatus>(_handle);

        // animation 更新
        Material* material = GetComponent<Material>(_handle);
        float t            = currentInvisibleTime / playerStatus->GetInvincibilityTime();
        // 0~1 の間で 点滅させる
        material->color_[A] = std::clamp(EaseInSine((sinf(t * kAmplitude) * 0.5f) + 0.5f), 0.f, 1.f);
    } else {
        Material* material  = GetComponent<Material>(_handle);
        material->color_[A] = 1.f;
    }
}
