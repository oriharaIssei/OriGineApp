#include "FloatingFloorFallSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

/// app
// component
#include "component/Piller/FloatingFloorStatus.h"
#include"component/Menu/MenuStatus.h"

// system

FloatingFloorFallSystem::FloatingFloorFallSystem() : ISystem(SystemType::Movement) {}
FloatingFloorFallSystem::~FloatingFloorFallSystem() {}

void FloatingFloorFallSystem::Initialize() {
}

void FloatingFloorFallSystem::Finalize() {
    /*  entities_.clear();*/
}

void FloatingFloorFallSystem::UpdateEntity(GameEntity* _entity) {

      // ポーズ中は通さない
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");

    if (!menuEntity) {
        return;
    }

    MenuStatus* menu = getComponent<MenuStatus>(menuEntity);

    if (!menu) {
        return;
    }

    if (menu->GetIsPose()) {
        return;
    }

    FloatingFloorStatus* entityStatus = getComponent<FloatingFloorStatus>(_entity);

    if (!entityStatus) {
        return;
    }

    if (!entityStatus->GetIsFall()||entityStatus->GetIsDestroy()) {
        return;
    }

    Transform* entityTransform = getComponent<Transform>(_entity);

    // フロアの落下処理
    entityStatus->SetIncrementFallEaseT(Engine::getInstance()->getDeltaTime() * entityStatus->GetFallSpeed());

    // 落とす
    entityTransform->translate[Y] = Lerp(entityStatus->GetStartPosY(),entityStatus->GetFallPosY(),entityStatus->GetFallEaseT());

    // 落ちた後の処理
    if (entityStatus->GetFallEaseT() < 1.0f) {
        return;
    }

    // savePosY_を更新
    entityTransform->translate[Y] = entityStatus->GetFallPosY();
  /*  entityStatus->SetStartPosY(entityTransform->translate[Y]);*/
    entityStatus->SetFallEaseT(0.0f);

    // フラグ戻す
    entityStatus->SetIsFall(false);

    // 破壊条件
    if (entityTransform->translate[Y] <= entityStatus->GetFallPosY()) {
        entityStatus->SetIsDestroy(true);
    }
}

float FloatingFloorFallSystem::Lerp(const float& start, const float& end, float t) {
    return (1.0f - t) * start + end * t;
}
