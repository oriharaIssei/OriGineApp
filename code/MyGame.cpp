#include "MyGame.h"

/// engine include
#define ENGINE_INCLUDE
#define RESOURCE_DIRECTORY
#include <EngineInclude.h>
// directX12object
#include "directX12/RenderTexture.h"

// scene
#include "scene/Scene.h"
#include "scene/SceneManager.h"

// global variable
#include "globalVariables/GlobalVariables.h"

// input
#include "input/InputManager.h"

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
    sceneManager_ = std::make_unique<SceneManager>();

    variables_->LoadAllFile();
    engine_->Initialize();

    /// コマンドライン引数の処理
    auto commandLineItr = _commandLines.begin();
    while (commandLineItr != _commandLines.end()) {
        const std::string& commandLine = *commandLineItr;
        // シーン名の指定
        if (commandLine == "-s") {
            ++commandLineItr;
            if (commandLineItr == _commandLines.end()) {
                break;
            }
            // コマンドライン引数がある場合、シーン名を設定する
            const std::string& sceneName = *commandLineItr;
            sceneManager_->getStartupSceneNameRef().setValue(sceneName);
            LOG_DEBUG("GetCommandLine : {}", sceneName);
        }

        // 次の引数へ
        ++commandLineItr;
    }

    RegisterUsingComponents();
    RegisterUsingSystems();

    InputManager* inputManager = InputManager::getInstance();

    sceneManager_->Initialize(inputManager->getKeyboard(), inputManager->getMouse(), inputManager->getGamePad());

#ifdef _DEVELOP
    isRecording_ = true; //! TODO : CommandLineのなどで制御できるようにする
    recorder_    = std::make_unique<ReplayRecorder>();
    recorder_->Initialize(sceneManager_->getStartupSceneName());
    // 初期化時の入力を記録
    if (isRecording_ == true) {
        InputManager* inputManager = InputManager::getInstance();
        recorder_->RecordFrame(engine_->getDeltaTime(),
            inputManager->getKeyboard(),
            inputManager->getMouse(),
            inputManager->getGamePad());
    }
#endif // _DEVELOP
}

void MyGame::Finalize() {
    sceneManager_->Finalize();
    engine_->Finalize();
#ifdef _DEVELOP
    const std::string directory = kApplicationResourceDirectory + '/' + kReplayFolderName;
    recorder_->SaveToFile(directory);
#endif // _DEVELOP
}

void MyGame::Run() {
    while (true) {
        if (engine_->ProcessMessage() || sceneManager_->isExitGame()) {
            break;
        }
        engine_->BeginFrame();

#ifdef _DEVELOP
        if (isRecording_ == true) {
            // 入力の記録
            InputManager* inputManager = InputManager::getInstance();
            recorder_->RecordFrame(engine_->getDeltaTime(),
                inputManager->getKeyboard(),
                inputManager->getMouse(),
                inputManager->getGamePad());
        }
#endif // _DEVELOP

        // シーンの更新
        sceneManager_->Update();
        sceneManager_->Render();
        // windowに描画
        Engine::getInstance()->ScreenPreDraw();
        sceneManager_->getCurrentScene()->getSceneView()->DrawTexture();
        Engine::getInstance()->ScreenPostDraw();

        engine_->EndFrame();
    }
}
