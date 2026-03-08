#include "FallDetectionSystem.h"

/// ECS
// component
#include "component/Transform/Transform.h"

/// config
#include "component/player/PlayerConfig.h"

using namespace OriGine;

FallDetectionSystem::FallDetectionSystem() : ISystem(OriGine::SystemCategory::StateTransition) {}
FallDetectionSystem::~FallDetectionSystem() {}

void FallDetectionSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    Transform* transform = GetComponent<Transform>(_handle);

    if (!transform) {
        return;
    }

    if (transform->worldMat[3][Y] < AppConfig::Player::kFallThresholdY) {
        OnFall();
    }
}
