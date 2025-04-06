#include "MyGame.h"

/// engine
#include "Engine.h"

// scene
#include "scene/GameScene.h"
#include "scene/TitleScene.h"
#include "sceneManager/SceneManager.h"

/// lib
#include "globalVariables/GlobalVariables.h"

#ifdef _DEBUG

/// editor
#include "ECSEditor.h"
#include "module/editor/EditorGroup.h"
/// debugger
#include "ECSDebugger.h"
#include "module/debugger/DebuggerGroup.h"

#endif // DEBUG

MyGame::MyGame() {}

MyGame::~MyGame() {}

void MyGame::Initialize() {
    ///=================================================================================================
    // Game のための 初期化
    ///=================================================================================================
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
    // シーンの変更を適応するために 一度更新
    sceneManager_->Update();
#ifdef _DEBUG
    ///=================================================================================================
    // Editor の初期化
    ///=================================================================================================
    {
        EditorGroup* editorGroup             = EditorGroup::getInstance();
        std::unique_ptr<ECSEditor> ecsEditor = std::make_unique<ECSEditor>();
        editorGroup->addEditor(std::move(ecsEditor));
        std::unique_ptr<GlobalVariablesEditor> globalVariables = std::make_unique<GlobalVariablesEditor>();
        editorGroup->addEditor(std::move(globalVariables));

        // gorup Initialize
        editorGroup->Initialize();
    }

    ///=================================================================================================
    // Debugger の初期化
    ///=================================================================================================
    {
        DebuggerGroup* debuggerGroup                = DebuggerGroup::getInstance();
        std::unique_ptr<EntityDebugger> ecsDebugger = std::make_unique<EntityDebugger>();
        debuggerGroup->addDebugger(std::move(ecsDebugger));

        // gorup Initialize
        debuggerGroup->Initialize();
    }

#endif // DEBUG
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

#ifdef _DEBUG
        sceneManager_->DebugUpdate();
#endif // DEBUG

        sceneManager_->Update();
        sceneManager_->Draw();

        engine_->EndFrame();
    }
}
