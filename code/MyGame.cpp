#include "MyGame.h"

/// engine
#include "Engine.h"

// scene
#include "scene/TitleScene.h"
#include "scene/GameScene.h"
#include "sceneManager/SceneManager.h"

///lib
#include "globalVariables/GlobalVariables.h"

MyGame::MyGame() {}

MyGame::~MyGame() {}

void MyGame::Initialize() {
    engine_ = Engine::getInstance();

    variables_    = GlobalVariables::getInstance();
    sceneManager_ = SceneManager::getInstance();

    variables_->LoadAllFile();
    engine_->Initialize();
    sceneManager_->Initialize();

    // exe 上で 使用するscene
    sceneManager_->addScene("Title", []() { return std::make_unique<TitleScene>(); });
    sceneManager_->addScene("Game", []() { return std::make_unique<GameScene>(); });

    SerializedField<std::string> startupSceneName{"Settings", "Scene", "StartupSceneName"};
    sceneManager_->changeScene(startupSceneName);
}

void MyGame::Finalize() {
    sceneManager_->Finalize();
    engine_->Finalize();
}

void MyGame::Run() {
    while (true) {
        if (engine_->ProcessMessage()) {
            break;
        }
        engine_->BeginFrame();

        sceneManager_->Update();
        sceneManager_->Draw();

        engine_->EndFrame();
    }
}
