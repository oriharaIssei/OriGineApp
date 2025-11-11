#include "ButtonInputSystem.h"

/// Engine
#define ENGINE_ECS
#include "EngineInclude.h"
// ingput
#include "input/GamePadInput.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
// component
#include "component/Button.h"

void ButtonInputSystem::Initialize() {}

void ButtonInputSystem::Finalize() {}

void ButtonInputSystem::UpdateEntity(Entity* _entity) {
    KeyboardInput* keyInput = getScene()->getKeyboardInput();
    MouseInput* mouseInput  = getScene()->getMouseInput();
    GamePadInput* padInput  = getScene()->getGamePadInput();

    std::vector<Button>* buttons = getComponents<Button>(_entity);

    if (buttons == nullptr) {
        return;
    }

    for (auto& button : *buttons) {

        // リセット
        button.setHovered(false);
        button.setPressed(false);
        button.setReleased(false);

        /// ====================== check Keyboard Input ====================== ///
        //! Shortcut で キャンセルの方法が無い
        for (auto key : button.getShortcutKey()) {
            if (keyInput->isPress(key)) {
                button.setPressed(true);
            }
            if (keyInput->isRelease(key)) {
                button.setReleased(true);
            }
        }

        /// ====================== check Pad Input ====================== ///
        for (auto padButton : button.getShortcutPadButton()) {
            if (padInput->isPress(padButton)) {
                button.setPressed(true);
            }
            if (padInput->isRelease(padButton)) {
                button.setReleased(true);
            }
        }

        /// ====================== check Mouse Input ====================== ///
        SpriteRenderer* buttonSprite = getComponent<SpriteRenderer>(_entity);
        if (buttonSprite != nullptr) {
            const Vec2f& mousePos   = mouseInput->getPosition();
            const Vec2f& buttonPos  = buttonSprite->getTranslate();
            const Vec2f& buttonSize = buttonSprite->getSpriteBuff()->scale_;
            const Vec2f& anchor     = buttonSprite->getAnchorPoint();

            Vec2f buttonLeftTop     = buttonPos - buttonSize * anchor;
            Vec2f buttonRightBottom = buttonPos + buttonSize * (Vec2f(1.0f, 1.0f) - anchor);

            // マウスがボタンの範囲内にあるかどうかを判定
            bool isMouseOverButton = (mousePos[X] >= buttonLeftTop[X] && mousePos[X] <= buttonRightBottom[X] && mousePos[Y] >= buttonLeftTop[Y] && mousePos[Y] <= buttonRightBottom[Y]);

            button.setHovered(isMouseOverButton);

            // ボタンがホバーされている場合、クリック状態を更新
            if (!button.isHovered()) {
                return;
            }
            // クリック状態を更新
            if (button.isPressed()) {
                button.setReleased(mouseInput->isRelease(MouseButton::LEFT));
            } else {
                button.setPressed(mouseInput->isRelease(MouseButton::LEFT));
            }
        }
    }
}
