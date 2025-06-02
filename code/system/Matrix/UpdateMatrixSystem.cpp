#include "UpdateMatrixSystem.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// include

#include "component/transform/Transform.h"
#include "engine/EngineInclude.h"

UpdateMatrixSystem::UpdateMatrixSystem() : ISystem(SystemType::Movement) {}
UpdateMatrixSystem::~UpdateMatrixSystem() {}

void UpdateMatrixSystem::Initialize() {
}

void UpdateMatrixSystem::Finalize() {
}

void UpdateMatrixSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    Transform* baseTransform   = getComponent<Transform>(_entity, 1);
    Transform* entityTransform = getComponent<Transform>(_entity, 0);

    if (!baseTransform) {// base更新
        return;
    }

    baseTransform->Update();

    if (!entityTransform) {// child更新
        return;
    }

    entityTransform->Update();
}
