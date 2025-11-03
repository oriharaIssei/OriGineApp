#include "ApplyMouseConditionSystem.h"

/// engine
#include "input/Input.h"

/// component
#include "component/MouseCondition.h"

ApplyMouseConditionSystem::ApplyMouseConditionSystem() : ISystem(SystemCategory::StateTransition) {}
ApplyMouseConditionSystem::~ApplyMouseConditionSystem() = default;

void ApplyMouseConditionSystem::Initialize() {}
void ApplyMouseConditionSystem::Finalize() {}

void ApplyMouseConditionSystem::UpdateEntity(Entity* _entity) {
    auto* conditions = getComponents<MouseCondition>(_entity);
    if (conditions == nullptr) {
        return;
    }

    Input* input = Input::getInstance();
    for (auto& condition : *conditions) {
        if (!condition.isActive()) {
            continue;
        }

        input->ShowMouseCursor(condition.isShowCursor());

        if (condition.isFixCursor()) {
            // マウスの座標を指定し続けることで、実質的にマウスを固定する
            input->setMousePos(condition.fixCursorPos());
        }
    }
}
