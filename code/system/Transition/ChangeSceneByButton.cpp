#include "ChangeSceneByButton.h"

/// engine
#define ENGINE_SCENE
#include "EngineInclude.h"

/// application
// component
#include "component/Button.h"
#include "component/SceneChanger.h"

ChangeSceneByButton::ChangeSceneByButton()
    : ISystem(SystemCategory::StateTransition) {}

ChangeSceneByButton::~ChangeSceneByButton() {}

void ChangeSceneByButton::Initialize() {
}

void ChangeSceneByButton::Finalize() {
}

void ChangeSceneByButton::UpdateEntity(GameEntity* _entity) {
    Button* button = getComponent<Button>(_entity);

    if (button->isReleased()) {
        SceneChanger* sceneChanger = getComponent<SceneChanger>(_entity);
        SceneManager::getInstance()->changeScene(sceneChanger->getNextSceneName());
    }
}
