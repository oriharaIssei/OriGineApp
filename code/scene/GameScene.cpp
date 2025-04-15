#include "GameScene.h"

/// ECS
#include "ECS/ECSManager.h"
// component
#include"Application/code/component/Player/PlayerStates.h"
#include"Application/code/component/Floor/BottomFloorStates.h"
#include"Application/code/component/Floor/FloorStates.h"
#include"Application/code/component/Piller/PillerStatus.h"
#include"Application/code/component/Piller/PillerSpawner.h"
#include"Application/code/component/Bom/BomSpawner.h"
#include"Application/code/component/Floor/FloorModeCreater.h"
#include"Application/code/component/Bom/BomStatus.h"
#include"Application/code/component/Bom/ExplotionCollision.h"
#include"Application/code/component/Combo/ComboStatus.h"
#include "Application/code/component/Combo/ComboUIStatus.h"
#include"Application/code/component/Timer/TimerUIStatus.h"
#include"Application/code/component/Timer/TimerStatus.h"

//system
#include"Application/code/system/Player/PlayerInput.h"
#include"Application/code/system/Player/PlayerMoveSystem.h"
#include"Application/code/system/Bom/PutBomSystem.h"
#include"Application/code/system/FloorAndPiller/TowerPositionSet.h"
#include"Application/code/system/Bom/BomExplotionSystem.h"
#include"Application/code/system/Bom/DeleteBomSystem.h"
#include"Application/code/system/Piller/PillerDamageSystem.h"
#include"Application/code/system/Piller/CanageStateFallSystem.h"
#include"Application/code/system/Bom/DeleteExplotionCollision.h"
#include"Application/code/system/Player/PlayerFollowCameraSystem.h"
#include"Application/code/system/Piller/DeletePillerEntitySystem.h"
#include"Application/code/system/FloorAndPiller/FloorAndPillerFallSystem.h"
#include"Application/code/system/Piller/PillerUpdateMatrixSystem.h"
#include"Application/code/system/Floor/DeleteFloorSystem.h"
#include"Application/code/system/Floor/FloorUpdateMatrixSystem.h"
#include"Application/code/system/Combo/ComboUIScrollSystem.h"
#include"Application/code/system/Combo/ComboSystem.h"
#include "Application/code/system/Timer/TimerDecrementSystem.h"
#include"Application/code/system/Timer/TimerUIScrollSystem.h"

GameScene::GameScene()
    : IScene("Game") {}

GameScene::~GameScene() {}

void GameScene::registerComponents() {
    IScene::registerComponents();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<PlayerStates>();
    ecsManager->registerComponent<BottomFloorStates>();
    ecsManager->registerComponent<FloorStates>();
    ecsManager->registerComponent<FloorAndPillerSpawner>();
    ecsManager->registerComponent<BomSpawner>();
    ecsManager->registerComponent<BomStatus>();
    ecsManager->registerComponent<FloorModeCreater>();
    ecsManager->registerComponent<ExplotionCollision>();
    ecsManager->registerComponent<FloorAndPillerrStatus>();
    ecsManager->registerComponent<ComboStatus>();
    ecsManager->registerComponent<ComboUIStatus>();
    ecsManager->registerComponent<TimerStatus>();
    ecsManager->registerComponent<TimerUIStatus>();
}

void GameScene::registerSystems() {
    IScene::registerSystems();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerSystem<PlayerInputSystem>();
    ecsManager->registerSystem<PlayerMoveSystem>();
    ecsManager->registerSystem<PutBomSystem>();
    ecsManager->registerSystem<TowerPositionSet>();
    ecsManager->registerSystem<BomExplotionSystem>();
    ecsManager->registerSystem<DeleteBomSystem>();
    ecsManager->registerSystem<PillerDamageSystem>();
    ecsManager->registerSystem<CanageStateFallSystem>();
    ecsManager->registerSystem<DeleteExplotionCollision>();
    ecsManager->registerSystem<PlayerFollowCameraSystem>();
    ecsManager->registerSystem<DeletePillerEntitySystem>();
    ecsManager->registerSystem<FloorAndPillerFallSystem>();
    ecsManager->registerSystem<PillerUpdateMatrixSystem>();
    ecsManager->registerSystem<DeleteFloorSystem>();
    ecsManager->registerSystem<FloorUpdateMatrixSystem>();
    ecsManager->registerSystem<ComboUIScrollSystem>();
    ecsManager->registerSystem<ComboSystem>();
    ecsManager->registerSystem<TimerDecrementSystem>();
    ecsManager->registerSystem<TimerUIScrollSystem>();
    ecsManager->SortPriorityOrderSystems();
}
