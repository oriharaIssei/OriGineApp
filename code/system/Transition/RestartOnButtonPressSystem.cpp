#include "RestartOnButtonPressSystem.h"

/// Engine
#include "messageBus/MessageBus.h"

/// ECS
// component
#include "component/ui/Button.h"

/// Event
#include "event/RestartEvent.h"

using namespace OriGine;

RestartOnButtonPressSystem::RestartOnButtonPressSystem() : ISystem(SystemCategory::StateTransition) {}
RestartOnButtonPressSystem::~RestartOnButtonPressSystem() {}

void RestartOnButtonPressSystem::Initialize() {}
void RestartOnButtonPressSystem::Finalize() {}

void RestartOnButtonPressSystem::UpdateEntity(OriGine::Entity* _entity) {
    // リスタートボタンが押されたらRestartEventを飛ばす
    auto* buttonComps = GetComponents<Button>(_entity);
    if (buttonComps == nullptr) {
        return;
    }
    for (auto& button : *buttonComps) {
        if (button.IsReleased()) {
            // リスタートイベントを飛ばす
            MessageBus::GetInstance()->Emit<RestartEvent>(RestartEvent());
            return;
        }
    }
}
