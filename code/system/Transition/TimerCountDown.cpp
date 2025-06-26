#include "TimerCountDown.h"

/// engine
#define DELTA_TIME
#include "EngineInclude.h"

/// ECS
#include "ECS/ECSManager.h"
// component
#include "component/TimerComponent.h"

TimerCountDown::TimerCountDown() : ISystem(SystemType::StateTransition) {}

TimerCountDown::~TimerCountDown() {}

void TimerCountDown::Initialize() {}

void TimerCountDown::Finalize() {}

void TimerCountDown::UpdateEntity(GameEntity* _entity) {
    auto timerComponent = getComponent<TimerComponent>(_entity);
    if (timerComponent) {
        timerComponent->setCurrentTime(timerComponent->getCurrentTime() - getMainDeltaTime());
    }
}
