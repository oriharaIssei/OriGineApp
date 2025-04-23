#include "GameScene.h"

/// ECS
#include "ECS/ECSManager.h"
// component
#include "component/cameraController/CameraController.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
// system
#include "system/Initialize/SettingGameCameraTarget.h"
#include "system/Input/CameraInputSystem.h"
#include "system/Input/PlayerInputSystem.h"
#include "system/Movement/FollowCameraUpdateSystem.h"
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

    // ----------- Camera ----------- //
    ecsManager->registerComponent<CameraController>();
}

void GameScene::registerSystems() {
    ECSManager* ecsManager = ECSManager::getInstance();
    IScene::registerSystems();

    // ----------- Player ----------- //
    ecsManager->registerSystem<PlayerInputSystem>();
    ecsManager->registerSystem<PlayerMoveSystem>();

    // ----------- Camera ----------- //
    ecsManager->registerSystem<SettingGameCameraTarget>();
    ecsManager->registerSystem<CameraInputSystem>();
    ecsManager->registerSystem<FollowCameraUpdateSystem>();

    ecsManager->SortPriorityOrderSystems();
}
