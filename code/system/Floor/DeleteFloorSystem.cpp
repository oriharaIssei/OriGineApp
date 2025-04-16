#include "DeleteFloorSystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Floor/FloorStates.h"
#include"component/Piller/PillerStatus.h"
#include "engine/EngineInclude.h"
#include <Vector.h>

DeleteFloorSystem::DeleteFloorSystem() : ISystem(SystemType::StateTransition) {}

void DeleteFloorSystem::Initialize() {
}

void DeleteFloorSystem::Finalize() {
    /*entities_.clear();*/
}

DeleteFloorSystem::~DeleteFloorSystem() {}




//void DeleteFloorSystem::UpdateEntity(GameEntity* _entity) {
//    if (!_entity) {
//        return;
//    }
//
//   Transform* transform = getComponent<Transform>(_entity);
//    FloorStates* floorStates = getComponent<FloorStates>(_entity);
//
//    if (!transform) {
//        return;
//    }
//
//    if (!transform->parent) {
//        DestroyEntity(_entity);
//        return;
//    }
//
//    // 床をデストロイ
//    if (transform->parent->scale == Vec3f{0.0f,0.0f,0.0f}) {  
//        DestroyEntity(_entity);
//    }
//
//    
//}
void DeleteFloorSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    FloorStates* floorStates = getComponent<FloorStates>(_entity);

    if (!floorStates) {
        return;
    }

    if (floorStates->GetParentStatus()->GetIsFall()) {
        DestroyEntity(_entity);
    }
}
