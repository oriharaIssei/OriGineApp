#include "GoalEffectInitialize.h"

/// component
#include "component/effect/MaterialEffectPipeLine.h"

GoalEffectInitialize::GoalEffectInitialize() : ISystem(SystemCategory::Initialize) {}
GoalEffectInitialize::~GoalEffectInitialize() {}

void GoalEffectInitialize::Initialize() {
    // 何もしない
}
void GoalEffectInitialize::Finalize() {
    // 何もしない
}

void GoalEffectInitialize::UpdateEntity(Entity* entity) {
    auto materialEffectPipeLine = GetComponent<MaterialEffectPipeLine>(entity);
    if (materialEffectPipeLine == nullptr) {
        return;
    }


}
