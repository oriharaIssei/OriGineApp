#include "ApplyMouseConditionSystem.h"

/// engine
#include "input/InputManager.h"

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

    MouseInput* mouseInput = InputManager::getInstance()->getMouse();

    for (auto& condition : *conditions) {
        if (!condition.isActive()) {
            continue;
        }

        mouseInput->ShowCursor(condition.isShowCursor());

        if (condition.isFixCursor()) {
            // マウスの座標を指定し続けることで、実質的にマウスを固定する
            // このシステムを使用して Mouse座標を固定することで Velocityを取得しながら Mouse座標を固定できる
            mouseInput->setPosition(condition.fixCursorPos());
        }
    }
}
