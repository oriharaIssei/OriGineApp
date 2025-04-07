#include "GameScene.h"

/// ECS
#include "ECS/ECSManager.h"
// component
#include"Application/code/component/Player/PlayerStates.h"
#include"Application/code/component/Floor/BottomFloorStates.h"
#include"Application/code/component/Floor/FloorStates.h"
#include"Application/code/component/Piller/PillerStates.h"
#include"Application/code/component/FloorAndPillerColum/FloorAndPillerSpawner.h"
#include"Application/code/component/Bom/BomSpawner.h"
#include"Application/code/component/Floor/FloorModeCreater.h"
#include"Application/code/component/Bom/BomStatus.h"
#include"Application/code/component/Bom/ExplotionCollision.h"
#include"Application/code/component/FloorAndPillerColum/FloorAndPillerrStatus.h"

//system
#include"Application/code/system/Player/PlayerInput.h"
#include"Application/code/system/Player/PlayerMoveSystem.h"
#include"Application/code/system/Bom/PutBomSystem.h"
#include"Application/code/system/FloorAndPiller/CreateFloorAndPillerSystem.h"
#include"Application/code/system/FloorAndPiller/TowerPositionSet.h"
#include"Application/code/system/Bom/BomExplotionSystem.h"
#include"Application/code/system/Bom/DeleteBomSystem.h"
#include"Application/code/system/Piller/PillerDamageSystem.h"
#include"Application/code/system/Piller/DeletePillerSystem.h"
#include"Application/code/system/Bom/DeleteExplotionCollision.h"

GameScene::GameScene()
    : IScene("Game") {}

GameScene::~GameScene() {}

void GameScene::registerComponents() {
    IScene::registerComponents();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<PlayerStates>();
    ecsManager->registerComponent<BottomFloorStates>();
    ecsManager->registerComponent<FloorStates>();
    ecsManager->registerComponent<PillerStates>();
    ecsManager->registerComponent<FloorAndPillerSpawner>();
    ecsManager->registerComponent<BomSpawner>();
    ecsManager->registerComponent<BomStatus>();
    ecsManager->registerComponent<FloorModeCreater>();
    ecsManager->registerComponent<ExplotionCollision>();
    ecsManager->registerComponent<FloorAndPillerrStatus>();
}

void GameScene::registerSystems() {
    IScene::registerSystems();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerSystem<PlayerInputSystem>();
    ecsManager->registerSystem<PlayerMoveSystem>();
    ecsManager->registerSystem<PutBomSystem>();
    ecsManager->registerSystem<CreateFloorAndPillerSystem>();
    ecsManager->registerSystem<TowerPositionSet>();
    ecsManager->registerSystem<BomExplotionSystem>();
    ecsManager->registerSystem<DeleteBomSystem>();
    ecsManager->registerSystem<PillerDamageSystem>();
    ecsManager->registerSystem<DeletePillerSystem>();
    ecsManager->registerSystem<DeleteExplotionCollision>();
    ecsManager->SortPriorityOrderSystems();
}
