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

    /// ====================== check Keyboard Input ====================== ///
    if (button != nullptr) {
        //! Shortcut で キャンセルの方法が無い
        for (auto key : button->getShortcutKey()) {
            if (input_->isPressKey(key)) {
                button->setPressed(true);
            }
            if (input_->isReleaseKey(key)) {
                button->setReleased(true);
            }
        }

        /// ====================== check Pad Input ====================== ///
        for (auto padButton : button->getShortcutPadButton()) {
            if (input_->isPressButton(padButton)) {
                button->setPressed(true);
            }
            if (input_->isReleaseButton(padButton)) {
                button->setReleased(true);
            }
        }
    }

    /// ====================== check Mouse Input ====================== ///
    SpriteRenderer* buttonSprite = getComponent<SpriteRenderer>(_entity);
    if (buttonSprite != nullptr) {
        const Vec2f& mousePos   = input_->getCurrentMousePos();
        const Vec2f& buttonPos  = buttonSprite->getTranslate();
        const Vec2f& buttonSize = buttonSprite->getSpriteBuff()->scale_;
        const Vec2f& anchor     = buttonSprite->getAnchorPoint();

        Vec2f buttonLeftTop     = buttonPos - buttonSize * anchor;
        Vec2f buttonRightBottom = buttonPos + buttonSize * (Vec2f(1.0f, 1.0f) - anchor);

        // マウスがボタンの範囲内にあるかどうかを判定
        bool isMouseOverButton = (mousePos[X] >= buttonLeftTop[X] && mousePos[X] <= buttonRightBottom[X] && mousePos[Y] >= buttonLeftTop[Y] && mousePos[Y] <= buttonRightBottom[Y]);

        button->setHovered(isMouseOverButton);

        if (!button->isHovered()) {
            return;
        }
        if (button->isPressed()) {
            button->setReleased(input_->isReleaseMouseButton(MouseButton::LEFT));
        } else {
            button->setPressed(input_->isTriggerMouseButton(MouseButton::LEFT));
        }
    }
}
