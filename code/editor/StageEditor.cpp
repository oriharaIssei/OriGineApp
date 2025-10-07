#include "StageEditor.h"

#ifdef _DEBUG

#include <string>

/// engine
#define RESOURCE_DIRECTORY
#include "EngineInclude.h"
// directX12
#include "directX12/RenderTexture.h"
// camera
#include "camera/CameraManager.h"

/// ECS
#include "system/ISystem.h"
// system
#include "system/Initialize/CreateStage.h"
#include "system/render/StageDebugRender.h"

/// editor
#include "editor/ControlEditArea.h"
#include "editor/EditorController.h"
#include "editor/LinkEditArea.h"
#include "editor/StepEditArea.h"

/// externals
#include <imgui/imgui.h>

/// util
#include "myFileSystem/MyFileSystem.h"
#include "myGui/MyGui.h"
#include "nameof.h"

StageEditorWindow::StageEditorWindow() : Editor::Window(nameof<StageEditorWindow>()) {}

StageEditorWindow::~StageEditorWindow() {}

void StageEditorWindow::Initialize() {
    /// ========================================================
    // Scene の初期化
    /// ========================================================
    scene_ = std::make_unique<Scene>("StageEditor");
    scene_->InitializeSceneView();
    scene_->InitializeECS();

    /// ========================================================
    // 必要な Component と Systemだけを使用する
    /// ========================================================
    // component
    {
        auto* componentRepo = scene_->getComponentRepositoryRef();
        componentRepo->registerComponentArray(nameof<Stage>());
    }
    // system
    {
        auto* systemRunner = scene_->getSystemRunnerRef();
        systemRunner->registerSystem(nameof<CreateStage>());
        systemRunner->registerSystem(nameof<StageDebugRender>());
    }

    // Editor用 Entity 初期化
    int stageEntityID = scene_->getEntityRepositoryRef()->CreateEntity("Stage", true);
    scene_->addComponent(nameof<Stage>(), stageEntityID, true);
    scene_->registerSystem(nameof<StageDebugRender>(), 0, true);

    /// ========================================================
    // windowFlags の初期化
    /// ========================================================
    isMaximized_ = false;

    isFocused_ = false;
    isOpen_    = false;

    windowFlags_ = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
    /// ========================================================
    // Area の初期化
    /// ========================================================
    addArea(std::make_shared<StageViewArea>());
    addArea(std::make_shared<ControlPointEditArea>());
    addArea(std::make_shared<LinkEditArea>());
    addArea(std::make_shared<StepEditArea>());

    /// ========================================================
    // Menu の初期化
    /// ========================================================
    addMenu(std::make_shared<StageFileMenu>());
}

void StageEditorWindow::Finalize() {}

StageViewArea::StageViewArea() : Editor::Area(nameof<StageViewArea>()) {}
StageViewArea::~StageViewArea() {}

void StageViewArea::Initialize() {
    addRegion(std::make_shared<StageViewRegion>(this));
}
void StageViewArea::Finalize() {}

StageViewArea::StageViewRegion::StageViewRegion(StageViewArea* _parent)
    : Editor::Region(nameof<StageViewRegion>()), parentWindow_(_parent) {}
StageViewArea::StageViewRegion::~StageViewRegion() {}

void StageViewArea::StageViewRegion::Initialize() {
    debugCamera_ = std::make_unique<DebugCamera>();
    debugCamera_->Initialize();
}

void StageViewArea::StageViewRegion::DrawGui() {
    auto stageEditorWindow = EditorController::getInstance()->getWindow<StageEditorWindow>();
    currentScene_          = stageEditorWindow->getCurrentScene();
    if (!currentScene_) {
        LOG_ERROR("StageViewRegion::UpdateScene: No current scene found.");
        return;
    }

    // resize (必要なら)
    float windowSizeLengthSq = parentWindow_->areaSize_.lengthSq();
    if (windowSizeLengthSq <= 0.f && windowSizeLengthSq - currentScene_->getSceneView()->getTextureSize().lengthSq() <= 0.001f) {
        currentScene_->getSceneView()->Resize(parentWindow_->areaSize_);
    }

    if (parentWindow_->isFocused().current()) {
        debugCamera_->Update();
        // windowを動かないようにする
        stageEditorWindow->addWindowFlags(ImGuiWindowFlags_NoMove);
    } else {
        stageEditorWindow->removeWindowFlags(ImGuiWindowFlags_NoMove);
    }
    debugCamera_->DebugUpdate();

    DrawScene();
}

void StageViewArea::StageViewRegion::Finalize() {
    if (debugCamera_) {
        debugCamera_->Finalize();
        debugCamera_.reset();
    }
}

void StageViewArea::StageViewRegion::UpdateScene() {
    auto* systemRunner = currentScene_->getSystemRunnerRef();
    if (systemRunner) {
        systemRunner->UpdateCategory(SystemCategory::Initialize);
    } else {
        LOG_ERROR("StageViewRegion::UpdateScene: No system runner found in the current scene.");
    }
}

void StageViewArea::StageViewRegion::DrawScene() {
    CameraManager* cameraManager  = CameraManager::getInstance();
    CameraTransform prevTransform = cameraManager->getTransform();

    cameraManager->setTransform(debugCamera_->getCameraTransform());
    cameraManager->DataConvertToBuffer();

    // 描画
    auto* sceneView = currentScene_->getSceneView();
    sceneView->PreDraw();
    currentScene_->getSystemRunnerRef()->UpdateCategory(SystemCategory::Render);
    sceneView->PostDraw();

    cameraManager->setTransform(prevTransform);

    ImGui::Image(reinterpret_cast<ImTextureID>(currentScene_->getSceneView()->getBackBufferSrvHandle().ptr), parentWindow_->areaSize_.toImVec2());
}

#pragma region "Menu"
StageFileMenu::StageFileMenu() : Editor::Menu(nameof<StageFileMenu>()) {}
void StageFileMenu::Initialize() {
    addMenuItem(std::make_shared<StageFileSaveMenuItem>());
    addMenuItem(std::make_shared<StageFileLoadMenuItem>());
}
void StageFileMenu::Finalize() {}
StageFileMenu::~StageFileMenu() {}

StageFileSaveMenuItem::StageFileSaveMenuItem() : Editor::MenuItem(nameof<StageFileSaveMenuItem>()) {}
StageFileSaveMenuItem::~StageFileSaveMenuItem() {}
void StageFileSaveMenuItem::Initialize() {
    directory_ = kApplicationResourceDirectory + "/Stage";
    filename_  = "";
}
void StageFileSaveMenuItem::DrawGui() {
    auto* currentScene = EditorController::getInstance()->getWindow<StageEditorWindow>()->getCurrentScene();
    auto stage         = currentScene->getComponent<Stage>(currentScene->getUniqueEntity("Stage")->getID());

    this->isSelected_.set(ImGui::BeginMenu("Save Stage"));
    if (this->isSelected_.isTrigger()) {
        directory_ = kApplicationResourceDirectory + "/Stage";
        filename_  = stage->getFileName();
    }

    // 開いていたら表示
    if (this->isSelected_.current()) {

        // ファイルダイアログを開く
        ImGui::InputText("Directory", &directory_, ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("Search##StageFileSaveMenuItem")) {
            // ディレクトリ選択ダイアログを開く
            MyFileSystem::selectFolderDialog(kApplicationResourceDirectory + "/Stage", directory_);
            if (directory_.empty()) {
                LOG_ERROR("No directory selected.");
                directory_ = kApplicationResourceDirectory + "/Stage";
            }
        }

        if (ImGui::InputText("Filename", &filename_, ImGuiInputTextFlags_EnterReturnsTrue)) {
            // Enterキーでファイル名を確定
            if (filename_.empty()) {
                LOG_ERROR("Filename cannot be empty.");
                filename_ = stage->getFileName();
            }
        }
        if (ImGui::Button("Save")) {
            if ((directory_ == "") || (filename_ == "")) {
                // ディレクトリまたはファイル名が空の場合
                LOG_ERROR("Directory or filename is empty.");
                ImGui::EndMenu();
                return;
            }

            stage->SaveFile(directory_, filename_);
            stage->setDirectory(directory_);
            stage->setFileName(filename_);

            filename_ = stage->getFileName();
        }
        ImGui::EndMenu();
    }
}

void StageFileSaveMenuItem::Finalize() {
    directory_ = "";
    filename_  = "";
}

StageFileLoadMenuItem::StageFileLoadMenuItem() : Editor::MenuItem(nameof<StageFileLoadMenuItem>()) {}
StageFileLoadMenuItem::~StageFileLoadMenuItem() {}
void StageFileLoadMenuItem::Initialize() {
    directory_ = "/Stage";
    filename_  = "";
}
void StageFileLoadMenuItem::DrawGui() {
    if (ImGui::MenuItem("LoadStage")) {
        if (!myfs::selectFileDialog(kApplicationResourceDirectory, directory_, filename_, {"stage"}, true)) {
            directory_ = "/Stage";
            filename_  = "";
            return;
        }
        auto* currentScene = EditorController::getInstance()->getWindow<StageEditorWindow>()->getCurrentScene();
        auto stage         = currentScene->getComponent<Stage>(currentScene->getUniqueEntity("Stage")->getID());
        stage->LoadFile(kApplicationResourceDirectory + "/" + directory_, filename_);
    }
}
void StageFileLoadMenuItem::Finalize() {
    directory_ = "";
    filename_  = "";
}

#pragma endregion


#endif // _DEBUG
