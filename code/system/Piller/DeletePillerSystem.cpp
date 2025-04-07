#include "DeletePillerSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/FloorAndPillerColum/FloorAndPillerrStatus.h"
#include "component/Piller/PillerStates.h"
#include "engine/EngineInclude.h"

DeletePillerSystem::DeletePillerSystem() : ISystem(SystemType::StateTransition) {}

void DeletePillerSystem::Initialize() {
}

void DeletePillerSystem::Finalize() {
    /*  entities_.clear();*/
}

DeletePillerSystem::~DeletePillerSystem() {}

void DeletePillerSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    PillerStates* status          = getComponent<PillerStates>(_entity);
    FloorAndPillerrStatus* fAndPStatus = getComponent<FloorAndPillerrStatus>(_entity);

    if (!status ) {
        return;
    }

    // HPゼロで破壊
    if (status->GetCurrentHp() > 0) {
        return;
    }

    // 破壊の前にーーー落とす床を決める
    fAndPStatus->SetIsFall(true);

    fAndPStatus->GetRowNum();
    fAndPStatus->GetColumNum();

    //破壊
    DestroyEntity(_entity);
    
}
