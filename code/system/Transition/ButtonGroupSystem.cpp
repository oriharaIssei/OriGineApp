#include "ButtonGroupSystem.h"

/// engine
#include "scene/Scene.h"
// input
#include "input/GamepadInput.h"
#include "input/KeyboardInput.h"

/// component
#include "component/ui/Button.h"
#include "component/ui/ButtonGroup.h"

ButtonGroupSystem::ButtonGroupSystem() : ISystem(OriGine::SystemCategory::StateTransition) {}

void ButtonGroupSystem::Initialize() {}

void ButtonGroupSystem::Finalize() {}

void ButtonGroupSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    OriGine::KeyboardInput* keyInput    = GetScene()->GetKeyboardInput();
    OriGine::GamepadInput* gamePadInput = GetScene()->GetGamepadInput();

    auto* buttonGroup = GetComponent<ButtonGroup>(_entity);
    if (buttonGroup == nullptr) {
        return;
    }

    int32_t currentButtonNumber = buttonGroup->GetCurrentButtonNumber();
    Button* currentButton       = GetComponent<Button>(GetEntity(buttonGroup->GetEntityId(currentButtonNumber)));
    if (!currentButton) {
        return;
    }
    currentButton->SetHovered(true);

    // 外部システムの入力に従う (ボタンのショートカットやマウスでの選択など)
    for (const auto& [index, entityID] : buttonGroup->GetButtonNumbers()) {
        auto* button = GetComponent<Button>(GetEntity(entityID));
        if (button == nullptr || button == currentButton) {
            continue;
        }

        bool isInput = button->isHovered() || button->isPressed() || button->IsReleased();

        // 選択されているボタンの更新
        if (isInput) {
            // 初期化
            if (currentButton != nullptr) {
                currentButton->SetHovered(false);
                currentButton->SetPressed(false);
                currentButton->SetReleased(false);
            }
            currentButtonNumber = index;
            buttonGroup->SetCurrentButtonNumber(currentButtonNumber);
            // 直接移動させたら return
            return;
        }
    }

    bool isPressed  = currentButton->isPressed();
    bool IsReleased = currentButton->IsReleased();

    // 決定ボタンの判定
    // pad が有効な場合は pad 優先
    if (gamePadInput->IsActive()) {
        for (const auto& button : buttonGroup->GetDecidePadButtons()) {
            if (isPressed) {
                if (gamePadInput->IsPress(button)) {
                    isPressed = true;
                    break;
                }
            } else {
                if (gamePadInput->IsRelease(button)) {
                    IsReleased = true;
                    break;
                }
            }
        }
    }
    if (!IsReleased) {
        for (const auto& key : buttonGroup->GetDecideKeys()) {
            if (isPressed) {
                if (keyInput->IsPress(key)) {
                    isPressed = true;
                    break;
                }
            } else {
                if (keyInput->IsTrigger(key)) {
                    IsReleased = true;
                    break;
                }
            }
        }
    }

    currentButton->SetPressed(isPressed);
    currentButton->SetReleased(IsReleased);

    if (IsReleased) {
        return;
    }

    // 選択ボタンの変更
    int32_t delta = 0;

    if (gamePadInput->IsActive()) {
        for (const auto& button : buttonGroup->GetSelectAddPadButtons()) {
            if (gamePadInput->IsTrigger(button)) {
                ++delta;
                break;
            }
        }
        for (const auto& button : buttonGroup->GetSelectSubPadButtons()) {
            if (gamePadInput->IsTrigger(button)) {
                --delta;
                break;
            }
        }
    }

    for (const auto& key : buttonGroup->GetSelectAddKeys()) {
        if (keyInput->IsTrigger(key)) {
            ++delta;
        }
    }
    for (const auto& key : buttonGroup->GetSelectSubKeys()) {
        if (keyInput->IsTrigger(key)) {
            --delta;
        }
    }

    if (delta != 0) {
        delta = std::clamp(delta, -1, 1);

        // 初期化
        currentButton->SetPressed(false);
        currentButton->SetHovered(false);

        // 次のボタンを取得
        currentButtonNumber += delta;
        currentButtonNumber = std::clamp(currentButtonNumber, 0, (int32_t)buttonGroup->GetButtonNumbers().size() - 1);

        currentButton = GetComponent<Button>(GetEntity(buttonGroup->GetEntityId(currentButtonNumber)));

        // 次のボタンが存在しなければ戻す
        if (!currentButton) {
            currentButtonNumber -= delta;
            currentButton = GetComponent<Button>(GetEntity(buttonGroup->GetEntityId(currentButtonNumber)));
        }
        currentButton->SetHovered(true);

        buttonGroup->SetCurrentButtonNumber(currentButtonNumber);
    }
}
