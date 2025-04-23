#include "FloatingFloorRevivalSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

/// app
// component
#include "component/Piller/FloatingFloorStatus.h"

// system

FloatingFloorRevivalSystem::FloatingFloorRevivalSystem() : ISystem(SystemType::Movement) {}
FloatingFloorRevivalSystem::~FloatingFloorRevivalSystem() {}

void FloatingFloorRevivalSystem::Initialize() {
}

void FloatingFloorRevivalSystem::Finalize() {
    /*  entities_.clear();*/
}

void FloatingFloorRevivalSystem::UpdateEntity(GameEntity* _entity) {

    FloatingFloorStatus* floatingStatus = getComponent<FloatingFloorStatus>(_entity);
    Transform* entityTransform          = getComponent<Transform>(_entity);
    AABBCollider* collider              = getComponent<AABBCollider>(_entity);
    if (!floatingStatus || !entityTransform || !collider) {
        return;
    }

   

    // 死んでからの復活処理
    if (!floatingStatus->GetIsRevaviling()) {
        return;
    }

    entityTransform->scale = {0.0f, 0.0f, 0.0f};
    entityTransform->Update();

    /// 　復活タイム加算
    floatingStatus->SetIncrementRevivalTime(Engine::getInstance()->getDeltaTime());

    // タイム終わりで復活
    if (floatingStatus->GetCurrentRevivalTime() < floatingStatus->GetRevivalTime()) {
        return;
    }

    floatingStatus->RevivalReset();
    entityTransform->translate[Y] = floatingStatus->GetStartPosY();
    entityTransform->scale = floatingStatus->GetSaveScale();
    collider->setActive(true);
     entityTransform->Update();

}
