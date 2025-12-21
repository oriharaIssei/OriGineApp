#include "ExitApplicationByButton.h"

/// engine
#include "scene/Scene.h"
#include "scene/SceneManager.h"
/// component
#include "component/ui/Button.h"

ExitApplicationByButton::ExitApplicationByButton() : ISystem(OriGine::SystemCategory::StateTransition) {}
ExitApplicationByButton::~ExitApplicationByButton() {}

void ExitApplicationByButton::UpdateEntity(OriGine::EntityHandle _handle) {
    auto buttons = GetComponents<Button>(_entity);
    if (buttons == nullptr) {
        return;
    }
    for (const auto& button : *buttons) {
        if (button.IsReleased()) {
            GetScene()->GetSceneManager()->SetExitGame(true);
            return;
        }
    }
}
