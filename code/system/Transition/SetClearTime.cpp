#include "SetClearTime.h"

/// component
#include "component/TimerComponent.h"
#include "system/Initialize/GetClearTime.h"

SetClearTime::SetClearTime() : ISystem(SystemCategory::StateTransition) {}
SetClearTime::~SetClearTime() {}

void SetClearTime::Initialize() {}
void SetClearTime::Finalize() {}

void SetClearTime::UpdateEntity(GameEntity* _entity) {
    if (_entity == nullptr) {
        return;
    }
    auto timerComp = getComponent<TimerComponent>(_entity);
    if (timerComp == nullptr) {
        return;
    }

    clearTime = timerComp->getCurrentTime();
}
