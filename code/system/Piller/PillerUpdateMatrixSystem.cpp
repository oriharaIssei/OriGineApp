#include "PillerUpdateMatrixSystem.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// include

#include "engine/EngineInclude.h"
#include"component/transform/Transform.h"


PillerUpdateMatrixSystem::PillerUpdateMatrixSystem() : ISystem(SystemType::Movement) {}
PillerUpdateMatrixSystem::~PillerUpdateMatrixSystem() {}

void PillerUpdateMatrixSystem::Initialize() {

}

void PillerUpdateMatrixSystem::Finalize() {

}


void PillerUpdateMatrixSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    Transform* baseTransform = getComponent<Transform>(_entity, 1);
    Transform* entityTransform = getComponent<Transform>(_entity, 0);

    baseTransform->Update();
    entityTransform->Update();
}
