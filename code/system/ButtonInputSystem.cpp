#include "ButtonInputSystem.h"

/// Engine
#define ENGINE_ECS
#include "EngineInclude.h"
// ingput
#include "input/Input.h"
// component
#include "component/Button.h"

void ButtonInputSystem::Initialize() {
    input_ = Input::getInstance();
}

void ButtonInputSystem::Finalize() {
}

void ButtonInputSystem::UpdateEntity(GameEntity* _entity) {
    Button* button = getComponent<Button>(_entity);
    if (button == nullptr) {
        return;
    }

    /// ====================== check Keyboard Input ====================== ///

    //! Shortcut で キャンセルの方法が無い
    for (auto key : button->getShortcutKey()) {
        if (input_->isTriggerKey(key)) {
            button->setPressed(true);
        }
        if (input_->isReleaseKey(key)) {
            button->setReleased(true);
        }
    }

    /// ====================== check Pad Input ====================== ///
    for (auto padButton : button->getShortcutPadButton()) {
        if (input_->isTriggerButton(padButton)) {
            button->setPressed(true);
        }
        if (input_->isReleaseButton(padButton)) {
            button->setReleased(true);
        }
    }

    /// ====================== check Mouse Input ====================== ///

    const Vec2f& mousePos   = input_->getCurrentMousePos();
    const Vec2f& buttonPos  = button->getSprite()->getSpriteBuff()->translate_;
    const Vec2f& buttonSize = button->getSprite()->getSpriteBuff()->scale_;
    const Vec2f& anchor     = button->getSprite()->getAnchorPoint();

    Vec2f buttonLeftTop     = buttonPos - buttonSize * anchor;
    Vec2f buttonRightBottom = buttonPos + buttonSize * (Vec2f(1.0f, 1.0f) - anchor);

    /// 範囲内に カーソルがあるか
    if (buttonLeftTop[X] <= mousePos[X] && mousePos[X] <= buttonRightBottom[X] && buttonLeftTop[Y] <= mousePos[Y] && mousePos[Y] <= buttonRightBottom[Y]) {
        button->setHovered(true);
    } else {
        button->setHovered(false);
    }

    if (!button->isHovered()) {
        button->setPressed(false);
        button->setReleased(false);
        return;
    }
    if (button->isPressed()) {
        button->setReleased(input_->isReleaseMouseButton(MouseButton::LEFT));
    } else {
        button->setPressed(input_->isTriggerMouseButton(MouseButton::LEFT));
    }
}
