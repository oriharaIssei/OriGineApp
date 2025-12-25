#include "ButtonScenePreviewSystem.h"

/// component
#include "component/SubScene.h"
#include "component/ui/ButtonGroup.h"

using namespace OriGine;

ButtonScenePreviewSystem::ButtonScenePreviewSystem() : ISystem(OriGine::SystemCategory::StateTransition) {}
ButtonScenePreviewSystem::~ButtonScenePreviewSystem() {}

void ButtonScenePreviewSystem::Initialize() {}
void ButtonScenePreviewSystem::Finalize() {}

void ButtonScenePreviewSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    auto buttonGroup = GetComponent<ButtonGroup>(_handle);

    // skip
    if (!buttonGroup) {
        return;
    }

    // 選択されているOriGine::Sceneを描画
    int32_t currentButtonNumber = buttonGroup->GetCurrentButtonNumber();
    auto& buttons               = buttonGroup->GetButtonNumbers();
    for (auto [buttonNumber, entityID] : buttons) {
        // buttonのエンティティを取得
        OriGine::Entity* buttonEnt = GetEntity(entityID);
        if (!buttonEnt) {
            continue;
        }
        // ボタンが持つシーンを取得
        auto subScene = GetComponent<SubScene>(_handle);

        // なければスキップ
        if (!subScene) {
            continue;
        }

        subScene->SetActive(currentButtonNumber == buttonNumber);
    }
}
