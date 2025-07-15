#include "PlayerMoveSystem.h"

/// engine
#define ENGINE_COMPONENTS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"

void PlayerMoveSystem::Initialize() {
}

void PlayerMoveSystem::Finalize() {
}

void PlayerMoveSystem::UpdateEntity(GameEntity* _entity) {
    PlayerStatus* playerStatus = getComponent<PlayerStatus>(_entity);

    const float deltaTime = getMainDeltaTime();
    playerStatus->getPlayerMoveState()->Update(deltaTime);
}
