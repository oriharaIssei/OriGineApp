#include "MyGame.h"

/// engine include
#define ENGINE_INCLUDE
#define ENGINE_ECS
#define ENGINE_COMPONENTS
#define ENGINE_SYSTEMS
#include <EngineInclude.h>

// scene
#include "scene/Scene.h"
#include "scene/SceneManager.h"

/// lib
#include "globalVariables/GlobalVariables.h"

#ifdef _DEBUG
/// editor
#include "editor/EditorController.h"
/// debugger
#include "logger/Logger.h"
#include "module/debugger/DebuggerGroup.h"
#endif // DEBUG

MyGame::MyGame() {}

MyGame::~MyGame() {}

void MyGame::Initialize(const std::string& _commandLine) {
    ///=================================================================================================
    // Game のための 初期化
    ///=================================================================================================
    variables_ = GlobalVariables::getInstance();

    engine_       = Engine::getInstance();
    sceneManager_ = SceneManager::getInstance();

    variables_->LoadAllFile();
    engine_->Initialize();

    if (!_commandLine.empty()) {
        // コマンドライン引数がある場合、シーン名を設定する
        sceneManager_->getStartupSceneNameRef().setValue(_commandLine);
        LOG_DEBUG("GetCommandLine : {}", _commandLine);
    } else {
        LOG_DEBUG("MyGame : No command line argument detected. Using default startup scene '{}'.", sceneManager_->getStartupSceneNameRef().GetValue()->c_str());
    }

    RegisterUsingComponents();
    RegisterUsingSystems();
    sceneManager_->Initialize();
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

        engine_->EndFrame();
    }
}
