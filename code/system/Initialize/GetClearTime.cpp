#include "GetClearTime.h"

/// component
#include "component/TimerComponent.h"

GetClearTime::GetClearTime() : ISystem(SystemCategory::Initialize) {}
GetClearTime::~GetClearTime() {}

void GetClearTime::Initialize() {}
void GetClearTime::Finalize() {}

void GetClearTime::UpdateEntity(GameEntity* _entity) {
    if (_entity == nullptr) {
        return;
    }
    auto timerComp = getComponent<TimerComponent>(_entity);
    if (timerComp == nullptr) {
        return;
    }

    timerComp->setMaxTime(clearTime);
    timerComp->setCurrentTime(clearTime);
}
