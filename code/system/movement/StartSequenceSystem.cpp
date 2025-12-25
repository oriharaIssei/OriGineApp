#include "StartSequenceSystem.h"

/// engine
#include "scene/Scene.h"

/// component
#include "component/TimerComponent.h"

/// system
#include "system/SystemRunner.h"

#include "system/Input/GhostInputUpdate.h"
#include "system/Input/PlayerInputSystem.h"
#include "system/Movement/PlayerMoveSystem.h"
#include "system/Transition/PenaltySystem.h"

/// util
#include "nameof.h"

StartSequenceSystem::StartSequenceSystem() : ISystem(OriGine::SystemCategory::Movement) {}
StartSequenceSystem::~StartSequenceSystem() {}

void StartSequenceSystem::Initialize() {
    stopSystems_ = {
        nameof<GhostInputUpdate>(),
        nameof<PlayerMoveSystem>(),
        nameof<PlayerInputSystem>(),
        nameof<PenaltySystem>()};
    TimerStartSequence();
}

void StartSequenceSystem::Finalize() {}

void StartSequenceSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    // すでに開始している場合は処理しない
    if (isStarted_) {
        return;
    }

    TimerStartSequence();

    auto* timerComp = GetComponent<TimerComponent>(_handle);
    if (timerComp == nullptr) {
        return;
    }

    OriGine::EntityHandle gametimerEntityHandle = GetUniqueEntity("Timer");
    TimerComponent* gameTimerComp               = GetComponent<TimerComponent>(gametimerEntityHandle);
    if (!gameTimerComp) {
        return;
    }
    // timerを止める
    gameTimerComp->SetStarted(false);

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

void StartSequenceSystem::TimerStartSequence() {
    // 特定のシステムを停止する
    auto* systemRunner = GetScene()->GetSystemRunnerRef();
    for (const auto& systemName : stopSystems_) {
        systemRunner->DeactivateSystem(systemName);
    }
}

void StartSequenceSystem::GameStartSequence() {
    // 停止していたシステムを再開する
    auto* systemRunner = GetScene()->GetSystemRunnerRef();
    for (const auto& systemName : stopSystems_) {
        systemRunner->RegisterSystem(systemName);
    }
}
