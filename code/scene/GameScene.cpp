#include "GameScene.h"

/// ECS
#include "ECS/ECSManager.h"
// component
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
// system
#include "system/Input/PlayerInputSystem.h"
#include "system/Movement/PlayerMoveSystem.h"

GameScene::GameScene()
    : IScene("Game") {}

GameScene::~GameScene() {}

void GameScene::registerComponents() {
    ECSManager* ecsManager = ECSManager::getInstance();
    IScene::registerComponents();

    // ----------- Player ----------- //
    ecsManager->registerComponent<PlayerInput>();
    ecsManager->registerComponent<PlayerStatus>();
}

void GameScene::registerSystems() {
    ECSManager* ecsManager = ECSManager::getInstance();
    IScene::registerSystems();

    // ----------- Player ----------- //
    ecsManager->registerSystem<PlayerInputSystem>();
    ecsManager->registerSystem<PlayerMoveSystem>();

    ecsManager->SortPriorityOrderSystems();
}
