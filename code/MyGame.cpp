#include "MyGame.h"

#include "Engine.h"
#include "globalVariables/GlobalVariables.h"

// scene
#include "scene/GameScene.h"
#include "sceneManager/SceneManager.h"

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
    sceneManager_->addScene("GameScene", []() { return std::make_unique<GameScene>(); });

    sceneManager_->changeScene("GameScene");
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

        variables_->Update();

        sceneManager_->Update();
        sceneManager_->Draw();

        engine_->EndFrame();
    }
}
