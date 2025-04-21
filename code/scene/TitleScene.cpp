#include "TitleScene.h"

/// engine
// ECS
#define ENGINE_ECS

#include "EngineInclude.h"

/// application
// component
#include "component/Button.h"
#include "component/SceneChanger.h"
// system
#include "system/Input/ButtonInputSystem.h"
#include "system/Transition/ChangeSceneByButton.h"
#include "system/Transition/UpdateButtonColorByState.h"

TitleScene::TitleScene() : IScene("Title") {}

TitleScene::~TitleScene() {}

void TitleScene::registerComponents() {
    IScene::registerComponents();
    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<Button>();
    ecsManager->registerComponent<SceneChanger>();

 
}

void TitleScene::registerSystems() {
    IScene::registerSystems();
    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerSystem<ButtonInputSystem>();
    ecsManager->registerSystem<UpdateButtonColorByState>();
    ecsManager->registerSystem<ChangeSceneByButton>();

   
    ecsManager->SortPriorityOrderSystems();
}
