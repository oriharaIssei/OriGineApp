#include "GameScene.h"

/// ECS
#include "ECS/ECSManager.h"
// component
#include "component/BulletSpawner.h"
#include "component/CharacterStatus.h"
// system
#include "system/CharacterOnCollision.h"
#include "system/DeleteCharacterEntitySystem.h"
#include "system/PlayerInputSystem.h"
#include "system/ShotBulletSystem.h"

GameScene::GameScene()
    : IScene("GameScene") {}

GameScene::~GameScene() {}

void GameScene::registerComponents() {
    IScene::registerComponents();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<BulletSpawner>();
    ecsManager->registerComponent<CharacterStatus>();
}

void GameScene::registerSystems() {
    IScene::registerSystems();

    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerSystem<CharacterOnCollision>();
    ecsManager->registerSystem<ShotBulletSystem>();
    ecsManager->registerSystem<PlayerInputSystem>();
    ecsManager->registerSystem<DeleteCharacterEntitySystem>();

    ecsManager->SortPriorityOrderSystems();
}
