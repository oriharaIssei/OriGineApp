#include "ExitApplicationByButton.h"

/// engine
#include "scene/SceneManager.h"
/// component
#include "component/Button.h"

ExitApplicationByButton::ExitApplicationByButton() : ISystem(SystemCategory::StateTransition) {}
ExitApplicationByButton::~ExitApplicationByButton() {}

void ExitApplicationByButton::UpdateEntity(Entity* _entity) {
    auto buttons = getComponents<Button>(_entity);
    if (buttons == nullptr) {
        return;
    }
    for (const auto& button : *buttons) {
        if (button.isReleased()) {
            getScene()->getSceneManager()->setExitGame(true);
            return;
        }
    }
}
