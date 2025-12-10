#include "ShowGameUIByInputDevice.h"

/// engine
#include "scene/Scene.h"
// input
#include "input/GamepadInput.h"

/// component
#include "component/renderer/Sprite.h"

using namespace OriGine;

ShowGameUIByInputDevice::ShowGameUIByInputDevice() : ISystem(OriGine::SystemCategory::StateTransition) {}

void ShowGameUIByInputDevice::Initialize() {}
void ShowGameUIByInputDevice::Finalize() {}

void ShowGameUIByInputDevice::UpdateEntity(OriGine::Entity* _entity) {
    bool isGamePad = GetScene()->GetGamepadInput()->IsActive();

    // ui Entity は keyUI, padUI の2つの SpriteRenderer を持っている
    auto keyUI = GetComponent<SpriteRenderer>(_entity);
    auto padUI = GetComponent<SpriteRenderer>(_entity, 1);

    keyUI->SetIsRender(!isGamePad);
    padUI->SetIsRender(isGamePad);
}
