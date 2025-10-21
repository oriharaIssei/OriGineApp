#include "TimerCountDown.h"

/// engine
#define DELTA_TIME
#include "EngineInclude.h"

/// ECS

// component
#include "component/TimerComponent.h"

TimerCountDown::TimerCountDown() : ISystem(SystemCategory::StateTransition) {}

TimerCountDown::~TimerCountDown() {}

void TimerCountDown::Initialize() {}

void TimerCountDown::Finalize() {}

void TimerCountDown::UpdateEntity(Entity* _entity) {
    auto timerComponent = getComponent<TimerComponent>(_entity);
    if (timerComponent) {
        timerComponent->setCurrentTime(timerComponent->getCurrentTime() - getMainDeltaTime());
    }
}
