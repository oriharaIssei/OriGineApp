#include "GameScene.h"

/// ECS
#include "ECS/ECSManager.h"
// component
#include"Application/code/component/Player/PlayerStates.h"
#include"Application/code/component/Field/FieldStates.h"
#include"Application/code/component/Floor/FloorStates.h"
#include"Application/code/component/Piller/PillerStates.h"
#include"Application/code/component/FloorAndPillerColum/FloorAndPillerColum.h"

//system
#include"Application/code/system/Player/PlayerInput.h"

GameScene::GameScene()
    : IScene("GameScene") {}

GameScene::~GameScene() {}

void GameScene::registerComponents() {
    IScene::registerComponents();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<PlayerStates>();
    ecsManager->registerComponent<FieldStates>();
    ecsManager->registerComponent<FloorStates>();
    ecsManager->registerComponent<PillerStates>();
    ecsManager->registerComponent<FloorAndPillerColum>();
}

void GameScene::registerSystems() {
    IScene::registerSystems();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerSystem<PlayerInputSystem>();


    ecsManager->SortPriorityOrderSystems();
}
