#include "SubSceneDeactivateByButton.h"

/// component
#include "component/Button.h"

SubSceneDeactivateByButton::SubSceneDeactivateByButton() : ISystem(SystemCategory::StateTransition) {}
SubSceneDeactivateByButton::~SubSceneDeactivateByButton() {}

void SubSceneDeactivateByButton::Initialize() {}
void SubSceneDeactivateByButton::Finalize() {}

void SubSceneDeactivateByButton::UpdateEntity(Entity* _entity) {
    auto* buttons = getComponents<Button>(_entity);
    if (!buttons) {
        return;
    }

    for (auto& button : *buttons) {
        if (button.isReleased()) {
            getScene()->setActive(false);
            return;
        }
    }
}
