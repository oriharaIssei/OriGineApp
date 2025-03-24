#include "TitleScene.h"

///engine
// ECS
#define ENGINE_ECS

#include "EngineInclude.h"

/// application
// component
#include "component/Button.h"
// system

TitleScene::TitleScene() : IScene("Title") {}

TitleScene::~TitleScene() {}

void TitleScene::registerComponents() {
    IScene::registerComponents();
    ECSManager* ecsManager = ECSManager::getInstance();
    ecsManager->registerComponent<Button>();
}

void TitleScene::registerSystems() {
    IScene::registerSystems();
}
