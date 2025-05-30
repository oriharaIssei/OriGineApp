#include "DeleteEntitySystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include

// component
#include "component/DeleteEntityStatus/DeleteEntityStatus.h"

#include "engine/EngineInclude.h"


DeleteEntitySystem::DeleteEntitySystem()
    : ISystem(SystemType::StateTransition) {}

DeleteEntitySystem::~DeleteEntitySystem() {}

void DeleteEntitySystem::Initialize() {
}

void DeleteEntitySystem::Finalize() {}

void DeleteEntitySystem::UpdateEntity(GameEntity* _entity) {

    // get timer component
    DeleteEntityStatus* deleteStatus = getComponent<DeleteEntityStatus>(_entity);

    if (!deleteStatus) {
        return;
    }
    float deltaTime = Engine::getInstance()->getDeltaTime();

    deleteStatus->DecrementTimer(deltaTime);

    if (deleteStatus->GetDeleteTime() <= 0.0f) {
        DestroyEntity(_entity);
    }
   
}

void DeleteEntitySystem::ComboReset() {
}

