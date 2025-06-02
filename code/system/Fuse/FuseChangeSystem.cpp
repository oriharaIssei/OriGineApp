#include "FuseChangeSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#include "myRandom/MyRandom.h"

#include <Vector3.h>
// component

#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockStatus.h"
#include "component/Fuse/FuseStatus.h"
#include "component/Menu/MenuStatus.h"
//  system
#include <cstdint>

FuseChangeSystem::FuseChangeSystem() : ISystem(SystemType::Movement) {}
FuseChangeSystem::~FuseChangeSystem() {}

void FuseChangeSystem::Initialize() {
    isInited_ = false;
}

void FuseChangeSystem::Finalize() {
}

void FuseChangeSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    // ポーズ中は通さない
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");
    GameEntity* bockCombiEntity              = ecsManager->getUniqueEntity("BlockCombination");

    if (!menuEntity || !bockCombiEntity) {
        return;
    }

    MenuStatus* menu        = getComponent<MenuStatus>(menuEntity);
    blockCombinationStatus_ = getComponent<BlockCombinationStatus>(bockCombiEntity);

    if (!menu || !blockCombinationStatus_) {
        return;
    }

    if (menu->GetIsPose()) {
        return;
    }

    FuseStatus* fuseStatus = getComponent<FuseStatus>(_entity);

    if (!fuseStatus) {
        return;
    }

    //* model
    ModelMeshRenderer* fuseCenterModel = getComponent<ModelMeshRenderer>(_entity);
    switch (*fuseStatus->GetFuseMode()) {
    case FuseMode::NONE:
        fuseCenterModel->setIsRender(false);
        break;

    case FuseMode::START:
        fuseCenterModel->setIsRender(true);
        CreateModelMeshRenderer(fuseCenterModel, _entity, kApplicationResourceDirectory + "/Models/Fuse", "Fuse.obj");


        break;

    case FuseMode::CENTER:
       /* fuseCenterModel->setIsRender(true);
        CreateModelMeshRenderer(fuseCenterModel, _entity, kApplicationResourceDirectory + "/Models/Fuse_Long", "Fuse_Long.obj");*/

        break;

    case FuseMode::END:
        break;
    default:
        break;
    }
}
