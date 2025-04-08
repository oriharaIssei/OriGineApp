#include "DeleteFloorAndPillerSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/FloorAndPillerColum/FloorAndPillerrStatus.h"
#include "component/Piller/PillerStates.h"
#include "engine/EngineInclude.h"

DeleteFloorAndPillerSystem::DeleteFloorAndPillerSystem() : ISystem(SystemType::StateTransition) {}

void DeleteFloorAndPillerSystem::Initialize() {
}

void DeleteFloorAndPillerSystem::Finalize() {
    /*  entities_.clear();*/
}

DeleteFloorAndPillerSystem::~DeleteFloorAndPillerSystem() {}




void DeleteFloorAndPillerSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    FloorAndPillerrStatus* fAndPStatus = getComponent<FloorAndPillerrStatus>(_entity);

    if (!fAndPStatus) {
        return;
    }

    // 柱床セットをデストロイ
    if (fAndPStatus->GetIsDestroy()) {
        DestroyEntity(_entity);
    }
    
    
}
