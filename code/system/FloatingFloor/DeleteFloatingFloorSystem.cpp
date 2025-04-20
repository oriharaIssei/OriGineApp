#include "DeleteFloatingFloorSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Piller/FloatingFloorSpawner.h"
#include "component/Piller/FloatingFloorStatus.h"
#include "engine/EngineInclude.h"

DeleteFloatingFloorSystem::DeleteFloatingFloorSystem() : ISystem(SystemType::StateTransition) {}

void DeleteFloatingFloorSystem::Initialize() {
}

void DeleteFloatingFloorSystem::Finalize() {
    /*  entities_.clear();*/
}

DeleteFloatingFloorSystem::~DeleteFloatingFloorSystem() {}


void DeleteFloatingFloorSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    FloatingFloorStatus* fAndPStatus = getComponent<FloatingFloorStatus>(_entity);

    if (!fAndPStatus) {
        return;
    }

    // 柱床セットをデストロイ
    if (fAndPStatus->GetIsDestroy()) {
    
        DestroyEntity(_entity);
    }
    
    
}
