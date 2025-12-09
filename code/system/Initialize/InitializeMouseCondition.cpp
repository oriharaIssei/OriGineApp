#include "InitializeMouseCondition.h"

/// engine
#include "input/MouseInput.h"

/// component
#include "component/MouseCondition.h"

InitializeMouseCondition::InitializeMouseCondition() : ISystem(SystemCategory::Initialize) {}

void InitializeMouseCondition::Initialize() {}
void InitializeMouseCondition::Finalize() {}

void InitializeMouseCondition::UpdateEntity(Entity* _entity) {
    auto* conditions = GetComponents<MouseCondition>(_entity);
    if (conditions == nullptr) {
        return;
    }

    MouseInput* mouseInput = GetScene()->GetMouseInput();

    for (auto& condition : *conditions) {
        if (!condition.IsActive()) {
            continue;
        }

        mouseInput->SetShowCursor(condition.IsShowCursor());

        if (condition.IsFixCursor()) {
            // マウスの座標を指定し続けることで、実質的にマウスを固定する
            // このシステムを使用して Mouse座標を固定することで Velocityを取得しながら Mouse座標を固定できる
            mouseInput->SetPosition(condition.GetFixCursorPos());
        }
    }
}
