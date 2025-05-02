#include "FloorUpdateMatrixSystem.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// include

#include "engine/EngineInclude.h"
#include"component/transform/Transform.h"


FloorUpdateMatrixSystem::FloorUpdateMatrixSystem() : ISystem(SystemType::Movement) {}
FloorUpdateMatrixSystem::~FloorUpdateMatrixSystem() {}

void FloorUpdateMatrixSystem::Initialize() {

}

void FloorUpdateMatrixSystem::Finalize() {

}


void FloorUpdateMatrixSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    Transform* entityTransform = getComponent<Transform>(_entity);

    entityTransform->Update();
}
