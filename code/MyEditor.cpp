#include "MyEditor.h"

#include "editor/EditorController.h"
#include "editor/StageEditor.h"
#include "editor/window/EditorWindowMenu.h"

#include "Engine.h"

MyEditor::MyEditor() {}
MyEditor::~MyEditor() {}

void MyEditor::Initialize(const std::string& /*_commandLine*/) {
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

    auto stageEditorWindow = std::make_unique<StageEditorWindow>();
    editorController_->addEditor<StageEditorWindow>(std::move(stageEditorWindow));

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
