#include "MyEditor.h"

#ifdef _DEBUG

/// editor
#include "editor/debugReplayer/DebugReplayWindow.h"
#include "editor/EditorController.h"
#include "editor/sceneEditor/SceneEditor.h"
#include "editor/setting/SettingWindow.h"
#include "editor/window/EditorWindowMenu.h"

#include "Engine.h"

MyEditor::MyEditor() {}
MyEditor::~MyEditor() {}

void MyEditor::Initialize(const std::vector<std::string>& _commandLines) {
    ///=================================================================================================
    // Editor のための 初期化
    ///=================================================================================================
    variables_        = GlobalVariables::getInstance();
    engine_           = Engine::getInstance();
    editorController_ = EditorController::getInstance();

    variables_->LoadAllFile();
    engine_->Initialize();

    RegisterUsingComponents();
    RegisterUsingSystems();

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
        editorController_->addEditor<DebugReplayWindow>(std::move(debugReplayWindow));
    } else {
        auto sceneEditorWindow = std::make_unique<SceneEditorWindow>();
        editorController_->addEditor<SceneEditorWindow>(std::move(sceneEditorWindow));

        auto settingWindow = std::make_unique<SettingWindow>();
        editorController_->addEditor<SettingWindow>(std::move(settingWindow));

        auto editorWindowMenu = std::make_unique<EditorWindowMenu>();
        editorWindowMenu->addMenuItem(std::make_shared<WindowItem<SceneEditorWindow>>());

        editorWindowMenu->addMenuItem(std::make_shared<WindowItem<SettingWindow>>());
        editorController_->addMainMenu<EditorWindowMenu>(std::move(editorWindowMenu));
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
