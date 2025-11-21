#include "PlayerMoveSystem.h"

/// engine
#define ENGINE_COMPONENTS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/Player/PlayerInput.h"
#include "component/Player/State/IPlayerMoveState.h"
#include "component/Player/State/PlayerState.h"

void PlayerMoveSystem::Initialize() {
}

void PlayerMoveSystem::Finalize() {
}

void PlayerMoveSystem::UpdateEntity(Entity* _entity) {
    PlayerState* state = GetComponent<PlayerState>(_entity);

    const float deltaTime = GetMainDeltaTime();
    auto moveState        = state->GetPlayerMoveState();
    if (moveState) {
        moveState->Update(deltaTime);
    }
}
