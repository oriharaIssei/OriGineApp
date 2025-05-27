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

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();

    FloatingFloorStatus* floatingStatus = getComponent<FloatingFloorStatus>(_entity);
    Transform* entityTransform          = getComponent<Transform>(_entity);
    AABBCollider* collider              = getComponent<AABBCollider>(_entity);
    GameEntity* FloorSound              = ecsManager->getUniqueEntity("FloorSound");
    Audio* revivalSound                 = getComponent<Audio>(FloorSound, 2);

    if (!floatingStatus || !entityTransform || !collider || !revivalSound) {
        return;
    }

    // 死んでからの復活処理
    if (!floatingStatus->GetIsRevaviling()) {
        return;
    }

    floatingStatus->SetBaseScale (Vec3f(0.0f, 0.0f, 0.0f));
    entityTransform->Update();

    /// 　復活タイム加算
    floatingStatus->SetIncrementRevivalTime(Engine::getInstance()->getDeltaTime());

    // タイム終わりで復活
    if (floatingStatus->GetCurrentRevivalTime() < floatingStatus->GetRevivalTime()) {
        return;
    }

    revivalSound->Play(); // 再生

    floatingStatus->RevivalReset();//リセット

    entityTransform->translate[Y] = floatingStatus->GetStartPosY();
    floatingStatus->SetIsRevivalAnimation(true);
    entityTransform->Update();

    collider->setActive(true);

        /*   entityTransform->scale        = floatingStatus->GetSaveScale();*/
}
