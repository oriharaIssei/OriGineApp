#include "DeletePillerEntitySystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/FloorAndPillerColum/FloorAndPillerrStatus.h"
#include "component/Piller/PillerStates.h"
#include "engine/EngineInclude.h"

DeletePillerEntitySystem::DeletePillerEntitySystem() : ISystem(SystemType::StateTransition) {}

void DeletePillerEntitySystem::Initialize() {
}

void DeletePillerEntitySystem::Finalize() {
    /*  entities_.clear();*/
}

DeletePillerEntitySystem::~DeletePillerEntitySystem() {}




void DeletePillerEntitySystem::UpdateEntity(GameEntity* _entity) {
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
