#include "ShowGameUIByInputDevice.h"

/// engine
#include "input/GamePadInput.h"

/// component
#include "component/renderer/Sprite.h"

ShowGameUIByInputDevice::ShowGameUIByInputDevice() : ISystem(SystemCategory::StateTransition) {}

void ShowGameUIByInputDevice::Initialize() {}
void ShowGameUIByInputDevice::Finalize() {}

void ShowGameUIByInputDevice::UpdateEntity(Entity* _entity) {
    bool isGamePad = getScene()->getGamePadInput()->isActive();

    // ui Entity は keyUI, padUI の2つの SpriteRenderer を持っている
    auto keyUI = getComponent<SpriteRenderer>(_entity);
    auto padUI = getComponent<SpriteRenderer>(_entity, 1);

    keyUI->setIsRender(!isGamePad);
    padUI->setIsRender(isGamePad);
}
