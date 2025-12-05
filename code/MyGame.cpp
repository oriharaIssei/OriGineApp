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
    variables_ = GlobalVariables::GetInstance();

    engine_       = Engine::GetInstance();
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
            sceneManager_->GetStartupSceneNameRef().SetValue(sceneName);
            LOG_DEBUG("GetCommandLine : {}", sceneName);
        }

        // 次の引数へ
        ++commandLineItr;
    }

    RegisterUsingComponents();
    RegisterUsingSystems();

    InputManager* inputManager = InputManager::GetInstance();

    sceneManager_->Initialize(inputManager->GetKeyboard(), inputManager->GetMouse(), inputManager->GetGamePad());

#ifdef _DEVELOP
    isRecording_ = true; //! TODO : CommandLineのなどで制御できるようにする
    recorder_    = std::make_unique<ReplayRecorder>();
    recorder_->Initialize(sceneManager_->GetStartupSceneName());
    // 初期化時の入力を記録
    if (isRecording_ == true) {
        recorder_->RecordFrame(engine_->GetDeltaTime(),
            inputManager->GetKeyboard(),
            inputManager->GetMouse(),
            inputManager->GetGamePad());
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
        if (engine_->ProcessMessage() || sceneManager_->IsExitGame()) {
            break;
        }
        engine_->BeginFrame();

#ifdef _DEVELOP
        if (isRecording_ == true) {
            // 入力の記録
            InputManager* inputManager = InputManager::GetInstance();
            recorder_->RecordFrame(engine_->GetDeltaTime(),
                inputManager->GetKeyboard(),
                inputManager->GetMouse(),
                inputManager->GetGamePad());
        }
#endif // _DEVELOP

        // シーンの更新
        sceneManager_->Update();
        // シーンの描画 (シーンが変更されたら描画しない)
        sceneManager_->Render();

        // windowに描画
        Engine::GetInstance()->ScreenPreDraw();
        sceneManager_->GetCurrentScene()->GetSceneView()->DrawTexture();
        Engine::GetInstance()->ScreenPostDraw();

        engine_->EndFrame();

        // シーン変更の実行
        if (sceneManager_->IsChangeScene()) {
            sceneManager_->ExecuteSceneChange();
        }
    }
}
