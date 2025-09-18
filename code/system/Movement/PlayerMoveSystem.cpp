#include "PlayerMoveSystem.h"

/// engine
#define ENGINE_COMPONENTS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/Player/PlayerInput.h"
#include "component/Player/State/PlayerState.h"
#include "component/Player/State/IPlayerMoveState.h"

void PlayerMoveSystem::Initialize() {
}

void PlayerMoveSystem::Finalize() {
}

void PlayerMoveSystem::UpdateEntity(GameEntity* _entity) {
    PlayerState* state= getComponent<PlayerState>(_entity);

    const float deltaTime = getMainDeltaTime();
    state->getPlayerMoveState()->Update(deltaTime);
}
