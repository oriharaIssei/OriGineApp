#include "ChangeSceneByButton.h"

/// engine
#define ENGINE_SCENE
#define ENGINE_ECS
#include "EngineInclude.h"

/// application
// component
#include "component/ui/Button.h"
#include "component/SceneChanger.h"

ChangeSceneByButton::ChangeSceneByButton()
    : ISystem(OriGine::SystemCategory::StateTransition) {}

ChangeSceneByButton::~ChangeSceneByButton() {}

void ChangeSceneByButton::Initialize() {}

void ChangeSceneByButton::Finalize() {}

void ChangeSceneByButton::UpdateEntity(OriGine::Entity* _entity) {
    Button* button = GetComponent<Button>(_entity);

    if (button->IsReleased()) {
        SceneChanger* sceneChanger = GetComponent<SceneChanger>(_entity);
        sceneChanger->ChangeScene();
    }
}
