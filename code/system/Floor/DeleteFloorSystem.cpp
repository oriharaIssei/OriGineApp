#include "DeleteFloorSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Floor/FloorStates.h"
#include"component/Piller/FloatingFloorStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

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

    FloorStates* floorStates = getComponent<FloorStates>(_entity);

    if (!floorStates) {
        return;
    }

    if (floorStates->GetParentStatus()->GetIsDestroy()) {
        DestroyEntity(_entity);
    }
}
