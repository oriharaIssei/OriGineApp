#include "StartSequenceSystem.h"

/// engine
#include "scene/Scene.h"

/// component
#include "component/effect/CameraAction.h"
#include "component/renderer/Sprite.h"
#include "component/TimerComponent.h"
#include "component/transform/CameraTransform.h"

/// system
#include "system/SystemRunner.h"

#include "system/Input/GhostInputUpdate.h"
#include "system/Input/PlayerInputSystem.h"
#include "system/Movement/PlayerMoveSystem.h"
#include "system/Transition/PenaltySystem.h"

#include "system/Input/CameraInputSystem.h"
#include "system/Movement/FollowCameraUpdateSystem.h"

/// util
#include "nameof.h"

using namespace OriGine;

StartSequenceSystem::StartSequenceSystem() : ISystem(OriGine::SystemCategory::Movement) {}
StartSequenceSystem::~StartSequenceSystem() {}

void StartSequenceSystem::Initialize() {
    stopSystemsInStartTimer_ = {
        nameof<CameraInputSystem>(),
        nameof<FollowCameraUpdateSystem>(),
    };

    stopSystems_ = {
        nameof<GhostInputUpdate>(),
        nameof<PlayerMoveSystem>(),
        nameof<PlayerInputSystem>(),
        nameof<PenaltySystem>()};

    StageIntroductionSequence();
}

void StartSequenceSystem::Finalize() {}

void StartSequenceSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    // すでに開始している場合は処理しない
    if (isStarted_) {
        return;
    }

    // ゲームタイマーを止める
    OriGine::EntityHandle gametimerEntityHandle = GetUniqueEntity("Timer");
    TimerComponent* gameTimerComp               = GetComponent<TimerComponent>(gametimerEntityHandle);
    if (gameTimerComp) {
        // timerを止める
        gameTimerComp->SetStarted(false);
    }

    // スタートタイマーの取得
    auto* timerComp = GetComponent<TimerComponent>(_handle);
    if (timerComp == nullptr) {
        return;
    }

    if (!isIntroductionSequenceEnd_) {
        EntityHandle gameCamera               = GetUniqueEntity("GameCamera");
        auto* gameCameraTransform             = GetComponent<CameraTransform>(gameCamera);
        gameCameraTransform->canUseMainCamera = false;

        EntityHandle introductionCameraHandle = GetUniqueEntity("StageIntroCamera");
        auto* cameraActionComp                = GetComponent<CameraAction>(introductionCameraHandle);

        timerComp->SetStarted(false);

        TimerForSpriteComponent* timer4SpriteComp = GetComponent<TimerForSpriteComponent>(_handle);
        if (timer4SpriteComp) {
            // スプライト用エンティティを非表示にする
            Entity* spriteEntity = GetEntity(timer4SpriteComp->GetSpritesEntityHandle());
            if (spriteEntity) {
                // スプライト用エンティティを非表示にする
                Entity* spriteEntity = GetEntity(timer4SpriteComp->GetSpritesEntityHandle());
                if (spriteEntity) {
                    auto& spriteComps = GetComponents<SpriteRenderer>(spriteEntity->GetHandle());
                    for (auto& sprite : spriteComps) {
                        sprite.SetIsRender(false);
                    }
                }
            }
        }

        if (cameraActionComp) {
            cameraActionComp->SetIsLoop(false);
            if (cameraActionComp->IsEnd()) {
                if (timer4SpriteComp) {
                    // スプライト用エンティティを非表示にする
                    Entity* spriteEntity = GetEntity(timer4SpriteComp->GetSpritesEntityHandle());
                    if (spriteEntity) {
                        auto& spriteComps = GetComponents<SpriteRenderer>(spriteEntity->GetHandle());
                        for (auto& sprite : spriteComps) {
                            sprite.SetIsRender(true);
                        }
                    }
                }

                // ゲームカメラを無効化
                gameCameraTransform->canUseMainCamera = true;
                auto* introductionCameraTransform     = GetComponent<CameraTransform>(introductionCameraHandle);
                if (introductionCameraTransform) {
                    introductionCameraTransform->canUseMainCamera = false;
                }

                // スタートタイマー中に動かすシステムを開始する
                TimerStartSequence();
                // スタートタイマーを開始する
                timerComp->SetStarted(true);
            }
        }
    } else {
        // タイマーが0以下になったらシステムを開始する
        if (timerComp->GetTime() <= 0.f) {
            GameStartSequence();
            // ゲームタイマーを開始する
            gameTimerComp->SetStarted(true);

            isStarted_ = true;

            // 自分自身を削除する
            GetScene()->AddDeleteEntity(_handle);
            // sprite用エンティティも削除する
            TimerForSpriteComponent* timer4SpriteComp = GetComponent<TimerForSpriteComponent>(_handle);
            if (timer4SpriteComp) {
                GetScene()->AddDeleteEntity(timer4SpriteComp->GetSpritesEntityHandle());
            }
        }
    }
}

void StartSequenceSystem::StageIntroductionSequence() {
    // 特定のシステムを停止する
    auto* systemRunner = GetScene()->GetSystemRunnerRef();

    for (const auto& systemName : stopSystemsInStartTimer_) {
        systemRunner->DeactivateSystem(systemName);
    }

    for (const auto& systemName : stopSystems_) {
        systemRunner->DeactivateSystem(systemName);
    }
}

void StartSequenceSystem::TimerStartSequence() {
    isIntroductionSequenceEnd_ = true;

    // 特定のシステムを停止する
    auto* systemRunner = GetScene()->GetSystemRunnerRef();

    // スタートタイマー中に動かすシステムを再開する
    for (const auto& systemName : stopSystemsInStartTimer_) {
        systemRunner->ActivateSystem(systemName);
    }
}

void StartSequenceSystem::GameStartSequence() {
    // 停止していたシステムを再開する
    auto* systemRunner = GetScene()->GetSystemRunnerRef();
    for (const auto& systemName : stopSystems_) {
        systemRunner->RegisterSystem(systemName);
    }
}
