#include "ShowGameUIByInputDevice.h"

/// engine
#include "input/GamePadInput.h"

/// component
#include "component/renderer/Sprite.h"

ShowGameUIByInputDevice::ShowGameUIByInputDevice() : ISystem(SystemCategory::StateTransition) {}

void ShowGameUIByInputDevice::Initialize() {}
void ShowGameUIByInputDevice::Finalize() {}

void ShowGameUIByInputDevice::UpdateEntity(Entity* _entity) {
    bool isGamePad = GetScene()->GetGamePadInput()->IsActive();

    // ui Entity は keyUI, padUI の2つの SpriteRenderer を持っている
    auto keyUI = GetComponent<SpriteRenderer>(_entity);
    auto padUI = GetComponent<SpriteRenderer>(_entity, 1);

    keyUI->SetIsRender(!isGamePad);
    padUI->SetIsRender(isGamePad);
}
