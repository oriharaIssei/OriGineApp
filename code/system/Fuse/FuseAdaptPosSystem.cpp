#include "FuseAdaptPosSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#include "myRandom/MyRandom.h"

#include <Vector3.h>
// component

#include "component/Fuse/FuseStatus.h"
//  system
#include "system/Block/BackPlaneChangeCloseSystem.h"


FuseAdaptPosSystem::FuseAdaptPosSystem() : ISystem(SystemType::Movement) {}
FuseAdaptPosSystem::~FuseAdaptPosSystem() {}

void FuseAdaptPosSystem::Initialize() {
    isInited_ = false;
}

void FuseAdaptPosSystem::Finalize() {
}

void FuseAdaptPosSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    FuseStatus* fuseStatus = getComponent<FuseStatus>(_entity);
    Transform* transform  = getComponent<Transform>(_entity);

    if (!fuseStatus->GetBasePos()) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();

    fuseStatus->ScaleEasing(deltaTime);

    transform->translate = *fuseStatus->GetBasePos()+fuseStatus->GetOffsetPos();
    transform->scale     = fuseStatus->GetScale();
  
}

