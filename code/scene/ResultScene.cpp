#include "ResultScene.h"

/// engine
// ECS
#define ENGINE_ECS

#include "EngineInclude.h"

/// application
// component
#include "component/Button.h"
#include "component/SceneChanger.h"
#include"component/ResultUI/ResultUIParentStatus.h"
#include"component/ResultUI/ResultUIRankStatus.h"
#include"component/ResultUI/ResultUIScoreStatus.h"
#include"component/ResultUI/ResultFontStatus.h"

// system
#include "system/ButtonInputSystem.h"
#include "system/ChangeSceneByButton.h"
#include "system/UpdateButtonColorByState.h"
#include "system/UsingCameraSetSystem.h"
#include"system/ResultUI/ResultScoreAdaptSystem.h"
#include"system/ResultUI/ResultUIParentSystem.h"
#include"system/ResultUI/ResultFontSystem.h"
#include"system/ResultUI/ResultRankSystem.h"

ResultScene::ResultScene() : IScene("Result") {}

ResultScene::~ResultScene() {}

void ResultScene::registerComponents() {
    IScene::registerComponents();
    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<Button>();
    ecsManager->registerComponent<SceneChanger>();
    ecsManager->registerComponent<ResultUIParentStatus>();
    ecsManager->registerComponent<ResultUIRankStatus>();
    ecsManager->registerComponent<ResultUIScoreStatus>();
    ecsManager->registerComponent<ResultFontStatus>();
}

void ResultScene::registerSystems() {
    IScene::registerSystems();
    ECSManager* ecsManager = ECSManager::getInstance();

    ecsManager->registerSystem<ButtonInputSystem>();
    ecsManager->registerSystem<UpdateButtonColorByState>();
    ecsManager->registerSystem<ChangeSceneByButton>();
    ecsManager->registerSystem<UsingCameraSetSystem>();
    ecsManager->registerSystem<ResultScoreAdaptSystem>();
    ecsManager->registerSystem<ResultUIParentSystem>();
    ecsManager->registerSystem<ResultFontSystem>();
    ecsManager->registerSystem<ResultRankSystem>();
    ecsManager->SortPriorityOrderSystems();
}
