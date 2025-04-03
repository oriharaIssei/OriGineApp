#include "FloorAndPillerFallSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

/// app
// component

#include "component/FloorAndPillerColum/FloorAndPillerrStatus.h"

// system
#include "system/Piller/DeletePillerSystem.h"
#include "system/Piller/PillerDamageSystem.h"

FloorAndPillerFallSystem::FloorAndPillerFallSystem() : ISystem(SystemType::Movement) {}
FloorAndPillerFallSystem::~FloorAndPillerFallSystem() {}

void FloorAndPillerFallSystem::Initialize() {
}

void FloorAndPillerFallSystem::Finalize() {
    /*  entities_.clear();*/
}

void FloorAndPillerFallSystem::UpdateEntity(GameEntity* _entity) {

    FloorAndPillerrStatus* entityStatus = getComponent<FloorAndPillerrStatus>(_entity);
    /* FloorStates* floorStates             = getComponent<FloorStates>(_entity);*/

    if (!entityStatus) {
        return;
    }

    if (!entityStatus->GetIsFall()) {
        return;
    }

    switch (entityStatus->GetColumNum()) {

    case 0:  /// 1段目(床だけが落ちる段)
      
        break;

    case 1: /// 2段目
        break;

    case 2:/// 3段目
        break;

    default:
        break;
    }
}
