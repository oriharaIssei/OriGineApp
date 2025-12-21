#include "GetClearTime.h"

/// app
#include "manager/PlayerProgressStore.h"

/// component
#include "component/TimerComponent.h"

GetClearTime::GetClearTime() : ISystem(OriGine::SystemCategory::Initialize) {}
GetClearTime::~GetClearTime() {}

void GetClearTime::Initialize() {}
void GetClearTime::Finalize() {}

void GetClearTime::UpdateEntity(OriGine::EntityHandle _handle) {
    if (_entity == nullptr) {
        return;
    }
    auto timerComp = GetComponent<TimerComponent>(_entity);
    if (timerComp == nullptr) {
        return;
    }

    PlayerProgressStore* progressStore = PlayerProgressStore::GetInstance();
    float clearTime                    = progressStore->GetLastPlayStageProgressData().clearTime;

    timerComp->SetMaxTime(clearTime);
    timerComp->ResetCurrentTime();
}
