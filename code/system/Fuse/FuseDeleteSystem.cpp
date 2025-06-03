#include "FuseDeleteSystem.h"

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


FuseDeleteSystem::FuseDeleteSystem() : ISystem(SystemType::StateTransition) {}
FuseDeleteSystem::~FuseDeleteSystem() {}

void FuseDeleteSystem::Initialize() {
    isInited_ = false;
}

void FuseDeleteSystem::Finalize() {
}

void FuseDeleteSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    //// ポーズ中は通さない
    //EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    //GameEntity* bockCombiEntity              = ecsManager->getUniqueEntity("BlockCombination");


    //if (!menuEntity || !bockCombiEntity) {
    //    return;
    //}

    FuseStatus* fuseStatus = getComponent<FuseStatus>(_entity);
    ModelMeshRenderer* renderer = getComponent<ModelMeshRenderer>(_entity);

    if (!fuseStatus->GetIsDeath()) {
        renderer->setIsRender(false);
        DestroyEntity(_entity);
        return;
    }


    if (*fuseStatus->GetIsDeath()) {
        renderer->setIsRender(false);
        DestroyEntity(_entity);
    }

}

