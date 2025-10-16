#include "SubSceneActivateByButton.h"

// component
#include "component/Button.h"
#include "component/SubScene.h"

SubSceneActivateByButton::SubSceneActivateByButton() : ISystem(SystemCategory::StateTransition) {}
SubSceneActivateByButton::~SubSceneActivateByButton() {}

void SubSceneActivateByButton::Initialize() {}

void SubSceneActivateByButton::Finalize() {}

void SubSceneActivateByButton::UpdateEntity(GameEntity* _entity) {
    auto subScene = getComponent<SubScene>(_entity);
    // 多重初期化防止
    if (subScene->isActive()) {
        return;
    }

    auto buttons = getComponents<Button>(_entity);
    if (buttons == nullptr) {
        return;
    }
    for (auto& button : *buttons) {
        // ボタンが押されたらシーンをアクティブにする
        if (button.isReleased()) {
            subScene->Activate();
            return;
        }
    }
}
