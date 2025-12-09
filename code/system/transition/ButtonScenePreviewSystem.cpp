#include "ButtonScenePreviewSystem.h"

/// component
#include "component/ui/ButtonGroup.h"
#include "component/SubScene.h"

ButtonScenePreviewSystem::ButtonScenePreviewSystem() : ISystem(SystemCategory::StateTransition) {}
ButtonScenePreviewSystem::~ButtonScenePreviewSystem() {}

void ButtonScenePreviewSystem::Initialize() {}
void ButtonScenePreviewSystem::Finalize() {}

void ButtonScenePreviewSystem::UpdateEntity(Entity* _entity) {
    auto buttonGroup = GetComponent<ButtonGroup>(_entity);

    // skip
    if (!buttonGroup) {
        return;
    }

    // 選択されているSceneを描画
    int32_t currentButtonNumber = buttonGroup->GetCurrentButtonNumber();
    auto& buttons               = buttonGroup->GetButtonNumbers();
    for (auto [buttonNumber, entityID] : buttons) {
        // buttonのエンティティを取得
        Entity* buttonEnt = GetEntity(entityID);
        if (!buttonEnt) {
            continue;
        }
        // ボタンが持つシーンを取得
        auto subScene = GetComponent<SubScene>(buttonEnt);

        // なければスキップ
        if (!subScene) {
            continue;
        }

        if (currentButtonNumber == buttonNumber) {
            subScene->SetActive(true);
        } else {
            subScene->SetActive(false);
        }
    }
}
