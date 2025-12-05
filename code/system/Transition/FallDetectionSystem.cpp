#include "FallDetectionSystem.h"

/// ECS
// component
#include "component/player/state/PlayerState.h"
#include "component/Transform/Transform.h"

FallDetectionSystem::FallDetectionSystem() : ISystem(SystemCategory::StateTransition) {}

FallDetectionSystem::~FallDetectionSystem() {}

void FallDetectionSystem::Initialize() {}

void FallDetectionSystem::Finalize() {}

void FallDetectionSystem::UpdateEntity(Entity* _entity) {
    constexpr float kFallThresholdY = -26.0f;
    Transform* transform            = GetComponent<Transform>(_entity);

    if (!transform) {
        return;
    }

    if (transform->worldMat[3][Y] < kFallThresholdY) {
        // リスタートフラグを立てる
        PlayerState* playerState = GetComponent<PlayerState>(_entity);

        auto& stateFlag = playerState->GetStateFlagRef();
        stateFlag.SetCurrent(stateFlag.Current() | PlayerStateFlag::IS_RESTART);
    }
}
