#include "StartSequenceSystem.h"

/// component
#include "component/effect/CameraAction.h"
#include "component/TimerComponent.h"

/// system
#include "system/SystemRunner.h"

StartSequenceSystem::StartSequenceSystem() : ISystem(SystemCategory::Movement) {}
StartSequenceSystem::~StartSequenceSystem() {}

void StartSequenceSystem::Initialize() {
    stopSystems_ = {
        "PlayerInputSystem",
        "PlayerMoveSystem",
        "PenaltySystem"};
}

void StartSequenceSystem::Finalize() {}

void StartSequenceSystem::UpdateEntity(Entity* _entity) {
    // すでに開始している場合は処理しない
    if (isStarted_) {
        return;
    }

    TimerStartSequence();

    auto* timerComp = GetComponent<TimerComponent>(_entity);
    if (timerComp == nullptr) {
        return;
    }

    // timerを止める
    Entity* gameTimerEntity = GetUniqueEntity("Timer");
    if (gameTimerEntity) {
        TimerComponent* gameTimerComp = GetComponent<TimerComponent>(gameTimerEntity);
        if (gameTimerComp) {
            gameTimerComp->SetStarted(false);
        }
    }

    // タイマーが0以下になったらシステムを開始する
    if (timerComp->GetTime() <= 0.f) {
        GameStartSequence();
        // ゲームタイマーを開始する
        if (gameTimerEntity) {
            TimerComponent* gameTimerComp = GetComponent<TimerComponent>(gameTimerEntity);
            if (gameTimerComp) {
                gameTimerComp->SetStarted(true);
            }
        }

        isStarted_ = true;

        // 自分自身を削除する
        GetScene()->AddDeleteEntity(_entity->GetID());
        // sprite用エンティティも削除する
        TimerForSpriteComponent* timer4SpriteComp = GetComponent<TimerForSpriteComponent>(_entity);
        if (timer4SpriteComp) {
            GetScene()->AddDeleteEntity(timer4SpriteComp->GetSpritesEntityId());
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
