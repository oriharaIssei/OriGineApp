#include "SubSceneActivateByButton.h"

// component
#include "component/Button.h"
#include "component/SubScene.h"

SubSceneActivateByButton::SubSceneActivateByButton() : ISystem(SystemCategory::StateTransition) {}
SubSceneActivateByButton::~SubSceneActivateByButton() {}

void SubSceneActivateByButton::Initialize() {}

void SubSceneActivateByButton::Finalize() {}

void SubSceneActivateByButton::UpdateEntity(Entity* _entity) {
    auto subScene = GetComponent<SubScene>(_entity);
    // 多重初期化防止
    if (subScene->IsActive()) {
        return;
    }

    auto buttons = GetComponents<Button>(_entity);
    if (buttons == nullptr) {
        return;
    }
    for (auto& button : *buttons) {
        // ボタンが押されたらシーンをアクティブにする
        if (button.IsReleased()) {
            subScene->Activate();
            return;
        }
    }
}
