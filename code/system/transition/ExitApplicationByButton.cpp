#include "ExitApplicationByButton.h"

/// engine
#include "scene/SceneManager.h"
/// component
#include "component/ui/Button.h"

ExitApplicationByButton::ExitApplicationByButton() : ISystem(SystemCategory::StateTransition) {}
ExitApplicationByButton::~ExitApplicationByButton() {}

void ExitApplicationByButton::UpdateEntity(Entity* _entity) {
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
