#include "TitleScene.h"

/// engine
// ECS
#define ENGINE_ECS

#include "EngineInclude.h"

/// application
// component
#include "component/Button.h"
#include "component/SceneChanger.h"
#include"Application/code/component/SceneTransition/SceneTransition.h"
// system
#include "system/ButtonInputSystem.h"
#include "system/ChangeSceneByButton.h"
#include "system/UpdateButtonColorByState.h"
#include "system/UsingCameraSetSystem.h"
#include"Application/code/system/SceneTransitionSystem/SceneTransitionSystem.h"

TitleScene::TitleScene() : IScene("Title") {}

TitleScene::~TitleScene() {}

void TitleScene::registerComponents() {
    IScene::registerComponents();
    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<Button>();
    ecsManager->registerComponent<SceneChanger>();
    ecsManager->registerComponent<SceneTransition>();
}

void TitleScene::registerSystems() {
    IScene::registerSystems();
    ECSManager* ecsManager = ECSManager::getInstance();

    ecsManager->registerSystem<ButtonInputSystem>();

    ecsManager->registerSystem<UpdateButtonColorByState>();
    ecsManager->registerSystem<ChangeSceneByButton>();
    ecsManager->registerSystem<UsingCameraSetSystem>();
    ecsManager->registerSystem<SceneTransitionSystem>();

    ecsManager->SortPriorityOrderSystems();
}
