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

      EntityComponentSystemManager* ecsManager = ECSManager::getInstance();

    FloatingFloorStatus* fAndPStatus = getComponent<FloatingFloorStatus>(_entity);
    Transform* transform             = getComponent<Transform>(_entity);
    AABBCollider* collider           = getComponent<AABBCollider>(_entity);
    GameEntity* FloorSound           = ecsManager->getUniqueEntity("FloorSound");
    Audio* breakSound               = getComponent<Audio>(FloorSound, 1);

    if (!fAndPStatus || !transform || !collider || !breakSound) {
        return;
    }

    // 柱床セットをデストロイ
    if (fAndPStatus->GetIsDestroy()&&!fAndPStatus->GetIsRevaviling()) {
        fAndPStatus->SetIsRevivaling(true);
        collider->setActive(false);
        breakSound->Play();
       
    }
    
    
}
