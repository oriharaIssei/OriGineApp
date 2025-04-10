#include "DeletePillerEntitySystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/Piller/PillerSpawner.h"
#include "component/Piller/PillerStatus.h"
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
        Transform* entityTransform = getComponent<Transform>(_entity, 1);
        entityTransform->scale     = Vec3f(0.0f, 0.0f, 0.0f); /// スケールセロ
        entityTransform->Update();
        DestroyEntity(_entity);
    }
    
    
}
