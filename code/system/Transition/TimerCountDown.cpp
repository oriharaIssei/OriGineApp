#include "TimerCountDown.h"

/// engine
#define DELTA_TIME
#include "EngineInclude.h"

/// ECS

// component
#include "component/TimerComponent.h"

TimerCountDown::TimerCountDown() : ISystem(OriGine::SystemCategory::StateTransition) {}

TimerCountDown::~TimerCountDown() {}

void TimerCountDown::Initialize() {}

void TimerCountDown::Finalize() {}

void TimerCountDown::UpdateEntity(OriGine::EntityHandle _handle) {
    auto timerComponent = GetComponent<TimerComponent>(_handle);
    if (timerComponent) {
        if (timerComponent->IsStarted()) {
            timerComponent->SetCurrentTime(timerComponent->GetTime() - GetMainDeltaTime());
        }
    }
}
