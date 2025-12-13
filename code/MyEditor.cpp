#include "MyEditor.h"

#ifdef _DEBUG

/// engine
#define RESOURCE_DIRECTORY
#include <EngineInclude.h>
// scene
#include "scene/SceneJsonRegistry.h"

/// editor
#include "editor/debugReplayer/DebugReplayWindow.h"
#include "editor/EditorController.h"
#include "editor/sceneEditor/SceneEditor.h"
#include "editor/Setting/SettingWindow.h"
#include "editor/window/EditorWindowMenu.h"

#include "Engine.h"

using namespace OriGine;

MyEditor::MyEditor() {}
MyEditor::~MyEditor() {}

void MyEditor::Initialize(const std::vector<std::string>& _commandLines) {
    ///=================================================================================================
    // Editor のための 初期化
    ///=================================================================================================
    variables_        = OriGine::GlobalVariables::GetInstance();
    engine_           = OriGine::Engine::GetInstance();
    editorController_ = OriGine::EditorController::GetInstance();

    variables_->LoadAllFile();
    engine_->Initialize();

    RegisterUsingComponents();
    RegisterUsingSystems();

    SceneJsonRegistry::GetInstance()->LoadAllScene(kApplicationResourceDirectory + "/" + kSceneJsonFolder);

    /// コマンドライン引数の処理
    // -s シーン名 : 指定したシーンを開く
    // -rpd : デバッグリプレイヤーモードで起動
    bool isReplayerMode = false;
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
            LOG_DEBUG("GetCommandLine : {}", sceneName);
        } else if (commandLine == "-rpd") {
            isReplayerMode = true;
        }

        // 次の引数へ
        ++commandLineItr;
    }

    if (isReplayerMode) {
        auto debugReplayWindow = std::make_unique<DebugReplayWindow>();
        editorController_->AddEditor<DebugReplayWindow>(std::move(debugReplayWindow));
    } else {
        auto sceneEditorWindow = std::make_unique<SceneEditorWindow>();
        editorController_->AddEditor<SceneEditorWindow>(std::move(sceneEditorWindow));

        auto settingWindow = std::make_unique<SettingWindow>();
        editorController_->AddEditor<SettingWindow>(std::move(settingWindow));

        auto editorWindowMenu = std::make_unique<EditorWindowMenu>();
        editorWindowMenu->AddMenuItem(std::make_shared<WindowItem<SceneEditorWindow>>());

        editorWindowMenu->AddMenuItem(std::make_shared<WindowItem<SettingWindow>>());
        editorController_->AddMainMenu<EditorWindowMenu>(std::move(editorWindowMenu));
    }

    editorController_->Initialize();
}

void MyEditor::Finalize() {
    editorController_->Finalize();

    engine_->Finalize();
}

void MyEditor::Run() {
    while (true) {
        if (engine_->ProcessMessage() || isEndRequest_) {
            break;
        }
        engine_->BeginFrame();

        editorController_->Update();

        // すべてImGuiに描画する
        engine_->ScreenPreDraw();
        engine_->ScreenPostDraw();

        engine_->EndFrame();
    }
}

#endif
