#include "SubSceneDeactivateByButton.h"

/// engine
#include "scene/Scene.h"

/// component
#include "component/ui/Button.h"

SubSceneDeactivateByButton::SubSceneDeactivateByButton() : ISystem(OriGine::SystemCategory::StateTransition) {}
SubSceneDeactivateByButton::~SubSceneDeactivateByButton() {}

void SubSceneDeactivateByButton::Initialize() {}
void SubSceneDeactivateByButton::Finalize() {}

void SubSceneDeactivateByButton::UpdateEntity(OriGine::Entity* _entity) {
    auto* buttons = GetComponents<Button>(_entity);
    if (!buttons) {
        return;
    }

    for (auto& button : *buttons) {
        if (button.IsReleased()) {
            GetScene()->SetActive(false);
            return;
        }
    }
}
