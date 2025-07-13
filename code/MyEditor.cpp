#include "MyEditor.h"

#ifdef _DEBUG

#include "editor/EditorController.h"
#include "editor/sceneEditor/SceneEditor.h"
#include "editor/setting/SettingWindow.h"

#include "Engine.h"

MyEditor::MyEditor() {}
MyEditor::~MyEditor() {}

void MyEditor::Initialize(const std::string& _commandLine) {
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

    auto sceneEditorWindow = std::make_unique<SceneEditorWindow>();
    if (!_commandLine.empty()) {
        // コマンドライン引数がある場合、シーン名を設定する
        sceneEditorWindow->getEditSceneName().setValue(_commandLine);
    }

    editorController_->addEditor<SceneEditorWindow>(std::move(sceneEditorWindow));
    auto settingWindow = std::make_unique<SettingWindow>();
    editorController_->addEditor<SettingWindow>(std::move(settingWindow));

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
