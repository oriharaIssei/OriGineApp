#include "SubSceneActivateByButton.h"

// component
#include "component/ui/Button.h"
#include "component/SubScene.h"

using namespace OriGine;

SubSceneActivateByButton::SubSceneActivateByButton() : ISystem(OriGine::SystemCategory::StateTransition) {}
SubSceneActivateByButton::~SubSceneActivateByButton() {}

void SubSceneActivateByButton::Initialize() {}

void SubSceneActivateByButton::Finalize() {}

void SubSceneActivateByButton::UpdateEntity(OriGine::Entity* _entity) {
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
