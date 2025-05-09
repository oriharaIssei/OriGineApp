#include "GameScene.h"

/// ECS
#include "ECS/ECSManager.h"
// component
GameScene::GameScene()
    : IScene("Game") {}

GameScene::~GameScene() {}

void GameScene::registerComponents() {
    IScene::registerComponents();

}

void GameScene::registerSystems() {
    IScene::registerSystems();

    ECSManager* ecsManager = ECSManager::getInstance();

    ecsManager->SortPriorityOrderSystems();
}
