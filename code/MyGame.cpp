#include "MyGame.h"

/// engine include
#define ENGINE_INCLUDE
#define RESOURCE_DIRECTORY
#include <EngineInclude.h>

// scene
#include "scene/Scene.h"
#include "scene/SceneManager.h"

// global variable
#include "globalVariables/GlobalVariables.h"

#ifdef _DEVELOP
// debugReplay
#include "debugReplayer/ReplayRecorder.h"
#endif // _DEVELOP

/// component
#include "component/renderer/Sprite.h"
#include "component/SubScene.h"

#ifdef _DEBUG
/// editor
#include "editor/EditorController.h"
#include "logger/Logger.h"
#endif // DEBUG

MyGame::MyGame() {}

MyGame::~MyGame() {}

void MyGame::Initialize(const std::vector<std::string>& _commandLines) {
    ///=================================================================================================
    // Game のための 初期化
    ///=================================================================================================
    variables_ = GlobalVariables::getInstance();

    engine_       = Engine::getInstance();
    sceneManager_ = SceneManager::getInstance();

    variables_->LoadAllFile();
    engine_->Initialize();

    /// コマンドライン引数の処理
    for (const auto& commandLine : _commandLines) {
        if (!commandLine.empty()) {
            // コマンドライン引数がある場合、シーン名を設定する
            sceneManager_->getStartupSceneNameRef().setValue(commandLine);
            LOG_DEBUG("GetCommandLine : {}", commandLine);
        }
    }

    RegisterUsingComponents();
    RegisterUsingSystems();
    sceneManager_->Initialize();

#ifdef _DEVELOP
    recorder_ = std::make_unique<ReplayRecorder>();
    recorder_->Initialize(sceneManager_->getStartupSceneName());
#endif // _DEVELOP
}

void MyGame::Finalize() {
    sceneManager_->Finalize();
    engine_->Finalize();
#ifdef _DEVELOP
    constexpr const char* directory = "/Replay";
    recorder_->SaveToFile();
#endif // _DEVELOP
}

void MyGame::Run() {
    while (true) {
        if (engine_->ProcessMessage() || sceneManager_->isExitGame()) {
            break;
        }
        engine_->BeginFrame();

        sceneManager_->Update();

        engine_->EndFrame();
    }
}
