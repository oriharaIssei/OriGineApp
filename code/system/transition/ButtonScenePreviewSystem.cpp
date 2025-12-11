#include "ButtonScenePreviewSystem.h"

/// component
#include "component/ui/ButtonGroup.h"
#include "component/SubScene.h"

using namespace OriGine;

ButtonScenePreviewSystem::ButtonScenePreviewSystem() : ISystem(OriGine::SystemCategory::StateTransition) {}
ButtonScenePreviewSystem::~ButtonScenePreviewSystem() {}

void ButtonScenePreviewSystem::Initialize() {}
void ButtonScenePreviewSystem::Finalize() {}

void ButtonScenePreviewSystem::UpdateEntity(OriGine::Entity* _entity) {
    auto buttonGroup = GetComponent<ButtonGroup>(_entity);

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
