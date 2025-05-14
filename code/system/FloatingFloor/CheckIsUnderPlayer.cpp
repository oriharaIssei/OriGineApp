#include "CheckIsUnderPlayer.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// component
#include "component/Piller/FloatingFloorStatus.h"
#include "component/Player/PlayerStates.h"

CheckIsUnderPlayer::CheckIsUnderPlayer() : ISystem(SystemType::Collision) {}

void CheckIsUnderPlayer::Initialize() {
}

void CheckIsUnderPlayer::Finalize() {
    /*  entities_.clear();*/
}

CheckIsUnderPlayer::~CheckIsUnderPlayer() {}

void CheckIsUnderPlayer::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* PlayerEntity                 = ecsManager->getUniqueEntity("Player");

    if (!PlayerEntity) {
        return;
    }
    FloatingFloorStatus* floatingFloorStatus  = getComponent<FloatingFloorStatus>(_entity);
    Transform* floatingFloorTransform = getComponent<Transform>(_entity);
    Transform* playerTransform        = getComponent<Transform>(PlayerEntity,1);

    if (!playerTransform || !floatingFloorStatus || !floatingFloorTransform) {
        return;
    }

    const Vec3f& center    = floatingFloorTransform->translate;
    const Vec3f& min    = floatingFloorStatus->GetFallCollisionSizeMin();
    const Vec3f& max    = floatingFloorStatus->GetFallCollisionSizeMax();
    const Vec3f& playerpos = playerTransform->translate;

    // 判定領域（X軸のみ）
    float minX = center[X] + min[X];
    float maxX = center[X] + max[X];

    bool isInX = (playerpos[X] >= minX && playerpos[X] <= maxX);

    //フラグ立てる
    floatingFloorStatus->SetIsPlayerUnderTheFloor(isInX);

}

