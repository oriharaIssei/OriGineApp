#include "FloorAndPillerFallSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

/// app
// component
#include "component/Piller/PillerStatus.h"

// system

FloorAndPillerFallSystem::FloorAndPillerFallSystem() : ISystem(SystemType::Movement) {}
FloorAndPillerFallSystem::~FloorAndPillerFallSystem() {}

void FloorAndPillerFallSystem::Initialize() {
}

void FloorAndPillerFallSystem::Finalize() {
    /*  entities_.clear();*/
}

void FloorAndPillerFallSystem::UpdateEntity(GameEntity* _entity) {

    FloorAndPillerrStatus* entityStatus = getComponent<FloorAndPillerrStatus>(_entity);

    if (!entityStatus) {
        return;
    }

    if (!entityStatus->GetIsFall()) {
        return;
    }

    Transform* entityTransform = getComponent<Transform>(_entity,1);

    // フロアの落下処理
    entityStatus->SetIncrementFallEaseT(Engine::getInstance()->getDeltaTime() * entityStatus->GetFallSpeed());

    // 落とす
    entityTransform->translate[Y] = Lerp(entityStatus->GetSavePos(),entityStatus->GetFallPosY(),entityStatus->GetFallEaseT());

    // 落ちた後の処理
    if (entityStatus->GetFallEaseT() < 1.0f) {
        return;
    }

    /// columを下げる
    entityStatus->SetColumDecrement();

    // savePosY_を更新
    entityTransform->translate[Y] = entityStatus->GetFallPosY();
    entityStatus->SetSavePos(entityTransform->translate[Y]);
    entityStatus->SetFallEaseT(0.0f);

    // フラグ戻す
    entityStatus->SetIsFall(false);

    // 破壊条件
    if (entityStatus->GetColumNum() < 0) {
        entityStatus->SetIsDestroy(true);
    }
}

float FloorAndPillerFallSystem::Lerp(const float& start, const float& end, float t) {
    return (1.0f - t) * start + end * t;
}
