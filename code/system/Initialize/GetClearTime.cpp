#include "GetClearTime.h"

/// component
#include "component/Stage/Stage.h"
#include "component/TimerComponent.h"

GetClearTime::GetClearTime() : ISystem(SystemCategory::Initialize) {}
GetClearTime::~GetClearTime() {}

void GetClearTime::Initialize() {}
void GetClearTime::Finalize() {}

void GetClearTime::UpdateEntity(Entity* _entity) {
    if (_entity == nullptr) {
        return;
    }
    auto timerComp = GetComponent<TimerComponent>(_entity);
    if (timerComp == nullptr) {
        return;
    }

    timerComp->SetMaxTime(Stage::GetClearTime());
    timerComp->SetCurrentTime(Stage::GetClearTime());
}
