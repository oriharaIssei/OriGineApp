#include "TransitionSceneByTimer.h"

/// engine
#define DELTA_TIME
#include "EngineInclude.h"

/// ECS

// components
#include "component/TimerComponent.h"

TransitionSceneByTimer::TransitionSceneByTimer()
    : ISystem(SystemCategory::StateTransition) {}

TransitionSceneByTimer::~TransitionSceneByTimer() {}

void TransitionSceneByTimer::Initialize() {}

void TransitionSceneByTimer::Finalize() {}

void TransitionSceneByTimer::UpdateEntity(GameEntity* _entity) {
    auto timerComponent = getComponent<TimerComponent>(_entity);
    if (timerComponent) {
        if (!timerComponent->isStarted()) {
            return; // タイマーが開始されていない場合は何もしない
        }
        timerComponent->setCurrentTime(timerComponent->getCurrentTime() - getMainDeltaTime());
    }
}
