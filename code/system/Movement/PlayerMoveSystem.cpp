#include "PlayerMoveSystem.h"

/// engine
#define ENGINE_COMPONENTS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/player/PlayerInput.h"
#include "component/player/state/IPlayerMoveState.h"
#include "component/player/state/PlayerState.h"

void PlayerMoveSystem::Initialize() {
}

void PlayerMoveSystem::Finalize() {
}

void PlayerMoveSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    PlayerState* state = GetComponent<PlayerState>(_entity);

    const float deltaTime = GetMainDeltaTime();
    auto moveState        = state->GetPlayerMoveState();
    if (moveState) {
        moveState->Update(deltaTime);
    }
}
