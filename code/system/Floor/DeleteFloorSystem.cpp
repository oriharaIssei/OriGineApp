#include "DeleteFloorSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Floor/FloorStates.h"
#include "engine/EngineInclude.h"

DeleteFloorSystem::DeleteFloorSystem() : ISystem(SystemType::StateTransition) {}

void DeleteFloorSystem::Initialize() {
}

void DeleteFloorSystem::Finalize() {
    /*entities_.clear();*/
}

DeleteFloorSystem::~DeleteFloorSystem() {}




void DeleteFloorSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

   Transform* transform = getComponent<Transform>(_entity);

    if (!transform) {
        return;
    }

    // 床をデストロイ
    if (!transform->parent) {
        DestroyEntity(_entity);
    }
}
