#include "ResultScene.h"

/// engine
// ECS
#define ENGINE_ECS

#include "EngineInclude.h"

/// application
// component
#include "component/Button.h"
#include "component/SceneChanger.h"
// system
#include "system/ButtonInputSystem.h"
#include "system/ChangeSceneByButton.h"
#include "system/UpdateButtonColorByState.h"
#include "system/UsingCameraSetSystem.h"

ResultScene::ResultScene() : IScene("Result") {}

ResultScene::~ResultScene() {}

void ResultScene::registerComponents() {
    IScene::registerComponents();
    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<Button>();
    ecsManager->registerComponent<SceneChanger>();
}

void ResultScene::registerSystems() {
    IScene::registerSystems();
    ECSManager* ecsManager = ECSManager::getInstance();

    ecsManager->registerSystem<ButtonInputSystem>();

    ecsManager->registerSystem<UpdateButtonColorByState>();
    ecsManager->registerSystem<ChangeSceneByButton>();
    ecsManager->registerSystem<UsingCameraSetSystem>();

    ecsManager->SortPriorityOrderSystems();
}
