#include "PenaltySystem.h"

/// component
#include "component/player/state/PlayerState.h"
#include "component/TimerComponent.h"

PenaltySystem::PenaltySystem() : ISystem(SystemCategory::StateTransition) {}
PenaltySystem::~PenaltySystem() {}

void PenaltySystem::Initialize() {}
void PenaltySystem::Finalize() {}

void PenaltySystem::UpdateEntity(Entity* _entity) {
    auto* playerState = GetComponent<PlayerState>(_entity);

    // ペナルティ状態でなければ 処理しない
    if (!playerState->IsPenalty()) {
        return;
    };

    // ペナルティ状態を解除
    // マイナスする時間を取得
    float penaltyTime = playerState->SufferPenalty();

    // TimerComponent を取得 & ペナルティー分 時間をマイナス
    Entity* tierEntity = GetUniqueEntity("Timer");
    if (!tierEntity) {
        return;
    }
    TimerComponent* timer = GetComponent<TimerComponent>(tierEntity);
    timer->SetCurrentTime(timer->GetTime() - penaltyTime);
}
