#include "ButtonGroupSystem.h"

/// engine
#include "input/Input.h"

/// component
#include "component/Button.h"
#include "component/ButtonGroup.h"

ButtonGroupSystem::ButtonGroupSystem() : ISystem(SystemCategory::StateTransition) {}

void ButtonGroupSystem::Initialize() {}

void ButtonGroupSystem::Finalize() {}

void ButtonGroupSystem::UpdateEntity(GameEntity* _entity) {
    Input* input      = Input::getInstance();
    auto* buttonGroup = getComponent<ButtonGroup>(_entity);
    if (buttonGroup == nullptr) {
        return;
    }

    int32_t currentButtonNumber = buttonGroup->getCurrentButtonNumber();
    Button* currentButton       = getComponent<Button>(getEntity(buttonGroup->getEntityId(currentButtonNumber)));
    currentButton->setHovered(true);

    // 外部システムの入力に従う (ボタンのショートカットやマウスでの選択など)
    for (const auto& [entityID, index] : buttonGroup->getButtonNumbers()) {
        auto* button = getComponent<Button>(getEntity(entityID));
        if (button == nullptr || button == currentButton) {
            continue;
        }

        bool isInput = button->isHovered() || button->isPressed() || button->isReleased();

        // 選択されているボタンの更新
        if (isInput) {
            // 初期化
            if (currentButton != nullptr) {
                currentButton->setHovered(false);
                currentButton->setPressed(false);
                currentButton->setReleased(false);
            }
            currentButtonNumber = index;
            buttonGroup->setCurrentButtonNumber(currentButtonNumber);
            // 直接移動させたら return
            return;
        }
    }

    bool isPressed  = currentButton->isPressed();
    bool isReleased = currentButton->isReleased();

    // 決定ボタンの判定
    if (input->isPadActive()) {
        for (const auto& button : buttonGroup->getDecidePadButtons()) {
            if (isPressed) {
                if (input->isPressButton(button)) {
                    isPressed = true;
                    break;
                }
            } else {
                if (input->isReleaseButton(button)) {
                    isReleased = true;
                    break;
                }
            }
        }
    }
    if (!isReleased) {
        for (const auto& key : buttonGroup->getDecideKeys()) {
            if (isPressed) {
                if (input->isPressKey(key)) {
                    isPressed = true;
                    break;
                }
            } else {
                if (input->isTriggerKey(key)) {
                    isReleased = true;
                    break;
                }
            }
        }
    }

    currentButton->setPressed(isPressed);
    currentButton->setReleased(isReleased);

    if (isReleased) {
        return;
    }

    // 選択ボタンの変更
    int32_t delta = 0;

    if (input->isPadActive()) {
        for (const auto& button : buttonGroup->getSelectAddPadButtons()) {
            if (input->isTriggerButton(button)) {
                ++delta;
                break;
            }
        }
        for (const auto& button : buttonGroup->getSelectSubPadButtons()) {
            if (input->isTriggerButton(button)) {
                --delta;
                break;
            }
        }
    }

    for (const auto& key : buttonGroup->getSelectAddKeys()) {
        if (input->isTriggerKey(key)) {
            ++delta;
        }
    }
    for (const auto& key : buttonGroup->getSelectSubKeys()) {
        if (input->isTriggerKey(key)) {
            --delta;
        }
    }

    if (delta != 0) {
        delta = std::clamp(delta, -1, 1);

        // 初期化
        currentButton->setPressed(false);
        currentButton->setHovered(false);

        // 次のボタンを取得
        currentButtonNumber += delta;
        currentButtonNumber = std::clamp(currentButtonNumber, 0, (int32_t)buttonGroup->getButtonNumbers().size() - 1);

        currentButton = getComponent<Button>(getEntity(buttonGroup->getEntityId(currentButtonNumber)));
        currentButton->setHovered(true);

        buttonGroup->setCurrentButtonNumber(currentButtonNumber);
    }
}
