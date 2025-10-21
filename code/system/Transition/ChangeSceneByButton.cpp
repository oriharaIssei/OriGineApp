#include "ChangeSceneByButton.h"

/// engine
#define ENGINE_SCENE
#define ENGINE_ECS
#include "EngineInclude.h"

/// application
// component
#include "component/Button.h"
#include "component/SceneChanger.h"

ChangeSceneByButton::ChangeSceneByButton()
    : ISystem(SystemCategory::StateTransition) {}

ChangeSceneByButton::~ChangeSceneByButton() {}

void ChangeSceneByButton::Initialize() {
    sceneManager_ = SceneManager::getInstance();
}

void ChangeSceneByButton::Finalize() {
    sceneManager_ = nullptr;
}

void ChangeSceneByButton::UpdateEntity(Entity* _entity) {
    Button* button = getComponent<Button>(_entity);

    if (button->isReleased()) {
        SceneChanger* sceneChanger = getComponent<SceneChanger>(_entity);
        sceneChanger->ChangeScene();
    }
}
