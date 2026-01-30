#include "FallDetectionSystem.h"

/// engine
#include "messageBus/MessageBus.h"

/// event
#include "event/GamefailedEvent.h"

/// ECS
// component
#include "component/player/state/PlayerState.h"
#include "component/Transform/Transform.h"

using namespace OriGine;

FallDetectionSystem::FallDetectionSystem() : ISystem(OriGine::SystemCategory::StateTransition) {}
FallDetectionSystem::~FallDetectionSystem() {}

void FallDetectionSystem::Initialize() {}

void FallDetectionSystem::Finalize() {}

void FallDetectionSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    constexpr float kFallThresholdY = -50.f;
    Transform* transform            = GetComponent<Transform>(_handle);

    if (!transform) {
        return;
    }

    if (transform->worldMat[3][Y] < kFallThresholdY) {
        MessageBus::GetInstance()->Emit<GamefailedEvent>(GamefailedEvent());
    }
}
