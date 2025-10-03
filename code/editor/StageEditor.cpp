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
#include "editor/EditorController.h"

/// externals
#include <imgui/imgui.h>
#include <imgui/ImGuizmo/ImGuizmo.h>

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

ControlPointEditArea::ControlPointEditArea() : Editor::Area(nameof<ControlPointEditArea>()) {}
ControlPointEditArea::~ControlPointEditArea() {}

void ControlPointEditArea::Initialize() {
    addRegion(std::make_shared<ControlPointEditRegion>(this));
}
void ControlPointEditArea::Finalize() {
    Editor::Area::Finalize();
    // クリア
    editControlPoints_.clear();
    editLinks_.clear();
    clickedMousePos_  = Vec2f(0.f, 0.f);
    releasedMousePos_ = Vec2f(0.f, 0.f);
}

ControlPointEditArea::ControlPointEditRegion::ControlPointEditRegion(ControlPointEditArea* _parent)
    : Editor::Region(nameof<ControlPointEditRegion>()), parentArea_(_parent) {}

ControlPointEditArea::ControlPointEditRegion::~ControlPointEditRegion() {}

void ControlPointEditArea::ControlPointEditRegion::Initialize() {
}

void ControlPointEditArea::ControlPointEditRegion::DrawGui() {
    auto* currentScene = EditorController::getInstance()->getWindow<StageEditorWindow>()->getCurrentScene();
    if (!currentScene) {
        LOG_ERROR("ControlPointEditRegion::DrawGui: No current scene found.");
        return;
    }
    auto* stage         = currentScene->getComponent<Stage>(currentScene->getUniqueEntity("Stage"));
    auto& controlPoints = stage->getControlPointsRef();
    auto& links         = stage->getLinksRef();

    if (ImGui::Button("+ ControlPoint")) {
        // 制御点を追加する
        // 制御点の初期位置は(0, 0, 0)とする
        Stage::ControlPoint newPoint;
        auto command = std::make_unique<AddControlPointCommand>(stage, newPoint);
        EditorController::getInstance()->pushCommand(std::move(command));
    }
    if (ImGui::Button("+ Link")) {
        // リンクを追加するための制御点が必要
        if (parentArea_->editControlPoints_.size() >= 2) {
            int32_t prevCtlPoint = parentArea_->editControlPoints_.front().first;
            auto commandCombo    = std::make_unique<CommandCombo>();

            if (parentArea_->editControlPoints_.size() == 2) {
                Stage::Link newLink;
                newLink.from_ = prevCtlPoint;
                newLink.to_   = parentArea_->editControlPoints_[1].first;

                commandCombo->addCommand(std::make_shared<AddLinkCommand>(stage, newLink));
            } else {
                // 選択されている 制御点を 順に つなぐ
                for (int32_t i = 1; i < parentArea_->editControlPoints_.size(); ++i) {
                    Stage::Link newLink;
                    newLink.from_ = prevCtlPoint;
                    newLink.to_   = parentArea_->editControlPoints_[i].first;

                    prevCtlPoint = parentArea_->editControlPoints_[i].first;

                    commandCombo->addCommand(std::make_shared<AddLinkCommand>(stage, newLink));
                }
            }

            EditorController::getInstance()->pushCommand(std::move(commandCombo));
        }
    }

    int32_t startIndex = stage->getStartPointIndex();
    if (ImGui::InputInt("StartPointIndex", &startIndex)) {
        if (startIndex < 0 || startIndex >= static_cast<int32_t>(controlPoints.size())) {
            startIndex = -1; // 無効なインデックスは-1に設定
        }
        stage->setStartPointIndex(startIndex);
    }
    int32_t goalIndex = stage->getGoalPointIndex();
    if (ImGui::InputInt("GoalPointIndex", &goalIndex)) {
        if (goalIndex < 0 || goalIndex >= static_cast<int32_t>(controlPoints.size())) {
            goalIndex = -1; // 無効なインデックスは-1に設定
        }
        stage->setGoalPointIndex(goalIndex);
    }

    ImGui::Separator();
    if (ImGui::TreeNode("Control Points")) {
        if (ImGui::Button("Sort Z")) {
            // 制御点をZ座標でソートするコマンドを追加
            auto command = std::make_unique<SortControlPointsCommand>(stage, parentArea_);
            EditorController::getInstance()->pushCommand(std::move(command));
        }

        std::string label = "";
        auto& io          = ImGui::GetIO();
        for (int32_t i = 0; i < static_cast<int32_t>(controlPoints.size()); ++i) {
            auto& ctlPoint = controlPoints[i];
            ImGui::PushID(i);

            // 選択状態か判定
            bool isSelected = std::any_of(
                parentArea_->editControlPoints_.begin(), parentArea_->editControlPoints_.end(),
                [i](const auto& p) { return p.first == i; });

            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 1.0f, 1.0f)); // 選択色
            }

            if (ImGui::Button("X")) {
                // 制御点を削除する
                // 選択リストからも削除
                if (isSelected) {
                    auto deselectCommand = std::make_unique<DeselectControlPointCommand>(parentArea_, ctlPoint, i);
                    EditorController::getInstance()->pushCommand(std::move(deselectCommand));
                }
                auto removeCommand = std::make_unique<RemoveControlPointCommand>(stage, i);
                EditorController::getInstance()->pushCommand(std::move(removeCommand));

                ImGui::PopID();
                continue;
            }
            if (isSelected) {
                ImGui::PopStyleColor();
            }

            ImGui::SameLine();

            // 背景色変更
            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.5f, 1.0f, 0.3f));
            }

            label = "ControlPoint " + std::to_string(i);
            if (ImGui::Selectable(label.c_str(), isSelected, 0, ImVec2(0, 0))) {
                if (io.KeyShift) {
                    // すでに選択されていなければ追加
                    if (!isSelected) {
                        auto selectCommand = std::make_unique<SelectControlPointCommand>(parentArea_, ctlPoint, i);
                        EditorController::getInstance()->pushCommand(std::move(selectCommand));
                    }
                } else {
                    // Shiftなしなら単独選択
                    auto clearCommand = std::make_unique<ClearSelectedControlPointsCommand>(parentArea_);
                    EditorController::getInstance()->pushCommand(std::move(clearCommand));
                    auto selectCommand = std::make_unique<SelectControlPointCommand>(parentArea_, ctlPoint, i);
                    EditorController::getInstance()->pushCommand(std::move(selectCommand));
                }
            }
            if (isSelected) {
                ImGui::PopStyleColor();
            }

            DragGuiVectorCommand<3, float>(label, ctlPoint.pos_, 0.01f);
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Links")) {
        std::string label = "";
        auto& io          = ImGui::GetIO();
        for (int32_t i = 0; i < static_cast<int32_t>(links.size()); ++i) {
            auto& link = links[i];
            ImGui::PushID(i);

            // 選択状態か判定
            bool isSelected = std::any_of(
                parentArea_->editLinks_.begin(), parentArea_->editLinks_.end(),
                [i](const auto& p) { return p.first == i; });

            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 1.0f, 1.0f));
            }

            if (ImGui::Button("X")) {
                stage->removeLink(i);
                // 選択リストからも削除
                auto command = std::make_unique<RemoveLinkCommand>(stage, i);
                EditorController::getInstance()->pushCommand(std::move(command));

                if (isSelected) {
                    ImGui::PopStyleColor();
                }
                ImGui::PopID();
                continue;
            }
            if (isSelected) {
                ImGui::PopStyleColor();
            }

            ImGui::SameLine();

            // 背景色変更
            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.5f, 1.0f, 0.3f));
            }

            label = "Link " + std::to_string(i);
            if (ImGui::Selectable(label.c_str(), isSelected, 0, ImVec2(0, 0))) {
                if (io.KeyShift) {
                    if (!isSelected) {
                        auto selectCommand = std::make_unique<SelectLinkCommand>(parentArea_, link, i);
                        EditorController::getInstance()->pushCommand(std::move(selectCommand));
                    }
                } else {
                    // Shiftなしなら単独選択
                    auto clearCommand = std::make_unique<ClearSelectedLinksCommand>(parentArea_);
                    EditorController::getInstance()->pushCommand(std::move(clearCommand));
                    auto selectCommand = std::make_unique<SelectLinkCommand>(parentArea_, link, i);
                    EditorController::getInstance()->pushCommand(std::move(selectCommand));
                }
            }
            if (isSelected) {
                ImGui::PopStyleColor();
            }

            label = "FromPointIndex##Link " + std::to_string(i);
            DragGuiCommand(label, link.from_, 1, 0, int32_t(controlPoints.size()) - 1, "%d");
            label = "FromPointPos##Link " + std::to_string(i);
            if (link.from_ >= 0 && link.from_ < static_cast<int32_t>(controlPoints.size())) {
                DragGuiVectorCommand<3, float>(label, controlPoints[link.from_].pos_, 0.01f);
            } else {
                ImGui::Text("Invalid Index");
            }

            label = "ToPointIndex##Link " + std::to_string(i);
            DragGuiCommand(label, link.to_, 1, 0, int32_t(controlPoints.size()) - 1, "%d");
            label = "ToPointPos##Link " + std::to_string(i);
            if (link.to_ >= 0 && link.to_ < static_cast<int32_t>(controlPoints.size())) {
                DragGuiVectorCommand<3, float>(label, controlPoints[link.to_].pos_, 0.01f);
            } else {
                ImGui::Text("Invalid Index");
            }

            ImGui::Spacing();

            label = "Normal##Link " + std::to_string(i);
            if (DragGuiVectorCommand(label, link.normal_, 0.01f)) {
                link.normal_ = link.normal_.normalize();
            }
            DragGuiCommand("Height", link.height_, 0.01f);
            DragGuiCommand("Width", link.width_, 0.01f);
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
};

void ControlPointEditArea::ControlPointEditRegion::Finalize() {};

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

#pragma region "Commands"
AddControlPointCommand::AddControlPointCommand(Stage* stage, const Stage::ControlPoint& point)
    : stage_(stage), point_(point) {}

AddControlPointCommand::~AddControlPointCommand() {}

void AddControlPointCommand::Execute() {
    addedPointIdx_ = static_cast<int32_t>(stage_->getControlPoints().size());
    stage_->addControlPoint(point_);
}

void AddControlPointCommand::Undo() {
    stage_->removeControlPoint(addedPointIdx_);
}

RemoveControlPointCommand::RemoveControlPointCommand(Stage* stage, int32_t index)
    : stage_(stage), index_(index) {}

RemoveControlPointCommand::~RemoveControlPointCommand() {}

void RemoveControlPointCommand::Execute() {
    removedPoint_ = stage_->getControlPoints()[index_];
    stage_->removeControlPoint(index_);

    int32_t linkIndex = 0;
    for (auto& link : stage_->getLinksRef()) {
        if (link.to_ == index_) {
            link.to_ = -1; // 無効化
            hasControlPointLinksIndex_.emplace_back(linkIndex, true);
        } else if (link.to_ >= index_) {
            link.to_--;
        }
        if (link.from_ == index_) {
            link.from_ = -1; // 無効化
            hasControlPointLinksIndex_.emplace_back(linkIndex, false);
        } else if (link.from_ >= index_) {
            link.from_--;
        }
        linkIndex++;
    }
}

void RemoveControlPointCommand::Undo() {
    stage_->insertControlPoint(index_, removedPoint_);

    for (auto& link : stage_->getLinksRef()) {
        if (link.to_ >= index_) {
            link.to_++;
        }
        if (link.from_ >= index_) {
            link.from_++;
        }
    }
    for (const auto& [linkIdx, isTo] : hasControlPointLinksIndex_) {
        if (isTo) {
            stage_->getLinksRef()[linkIdx].to_ = index_;
        } else {
            stage_->getLinksRef()[linkIdx].from_ = index_;
        }
    }
}

ClearControlPointsCommand::ClearControlPointsCommand(Stage* stage)
    : stage_(stage) {}

void ClearControlPointsCommand::Execute() {
    removedPoints_ = stage_->getControlPoints();
    stage_->clearControlPoints();
}

void ClearControlPointsCommand::Undo() {
    for (const auto& point : removedPoints_) {
        stage_->addControlPoint(point);
    }
}

AddLinkCommand::AddLinkCommand(Stage* stage, const Stage::Link& link)
    : stage_(stage), link_(link) {}

AddLinkCommand::~AddLinkCommand() {}

void AddLinkCommand::Execute() {
    addedLinkIdx_ = static_cast<int32_t>(stage_->getLinks().size());
    stage_->addLink(link_);
}

void AddLinkCommand::Undo() {
    stage_->removeLink(addedLinkIdx_);
}

RemoveLinkCommand::RemoveLinkCommand(Stage* stage, int32_t index)
    : stage_(stage), index_(index) {}
void RemoveLinkCommand::Execute() {
    removedLink_ = stage_->getLinks()[index_];
    stage_->removeLink(index_);
}
void RemoveLinkCommand::Undo() {
    stage_->insertLink(index_, removedLink_);
}

ClearLinksCommand::ClearLinksCommand(Stage* stage)
    : stage_(stage) {}

void ClearLinksCommand::Execute() {
    removedLinks_ = stage_->getLinks();
    stage_->clearLinks();
}

void ClearLinksCommand::Undo() {
    for (const auto& link : removedLinks_) {
        stage_->addLink(link);
    }
}

void SortControlPointsCommand::Execute() {
    if (!indexedPoints_.empty()) {
        indexedPoints_.clear();
    }
    // ソート前のインデックスと制御点のペアを作成
    for (int32_t i = 0; i < stage_->controlPoints_.size(); ++i) {
        indexedPoints_.emplace_back(i, stage_->controlPoints_[i]);
    }

    // Zでソート
    std::sort(indexedPoints_.begin(), indexedPoints_.end(),
        [](const auto& a, const auto& b) {
            return a.second.pos_[Z] < b.second.pos_[Z];
        });

    // ソート後のインデックスバインドを作成（oldIdx -> newIdx）
    for (int32_t newIdx = 0; newIdx < indexedPoints_.size(); ++newIdx) {
        int32_t oldIdx                 = indexedPoints_[newIdx].first;
        indexBind_[oldIdx]             = newIdx;
        stage_->controlPoints_[newIdx] = indexedPoints_[newIdx].second;
    }

    // links_ のインデックスを変換
    for (auto& link : stage_->links_) {
        link.from_ = indexBind_[link.from_];
        link.to_   = indexBind_[link.to_];
    }

    // parentWindow_->editControlPoints_ のインデックスを変換
    for (auto& [index, ctlPoint] : parentWindow_->getEditControlPointsRef()) {
        index = indexBind_[index];
    }

    // start, goal index も変換
    if (stage_->getStartPointIndex() != -1) {
        stage_->setStartPointIndex(indexBind_[stage_->getStartPointIndex()]);
    }
    if (stage_->getGoalPointIndex() != -1) {
        stage_->setGoalPointIndex(indexBind_[stage_->getGoalPointIndex()]);
    }
}

void SortControlPointsCommand::Undo() {
    // 元のインデックスに戻す
    for (const auto& pair : indexedPoints_) {
        stage_->controlPoints_[pair.first] = pair.second;
    }
    // links_ のインデックスを元に戻す
    for (auto& link : stage_->links_) {
        link.from_ = indexBind_.at(link.from_);
        link.to_   = indexBind_.at(link.to_);
    }
    // parentWindow_->editControlPoints_ のインデックスを元に戻す
    for (auto& pair : parentWindow_->getEditControlPointsRef()) {
        pair.first = indexBind_.at(pair.first);
    }

    // start, goal index も元に戻す
    if (stage_->getStartPointIndex() != -1) {
        stage_->setStartPointIndex(indexBind_.at(stage_->getStartPointIndex()));
    }
    if (stage_->getGoalPointIndex() != -1) {
        stage_->setGoalPointIndex(indexBind_.at(stage_->getGoalPointIndex()));
    }
}

SelectControlPointCommand::SelectControlPointCommand(ControlPointEditArea* _parentArea, const Stage::ControlPoint& _ctlPoint, int32_t index)
    : parentArea_(_parentArea), controlPoint_(_ctlPoint), index_(index) {}
SelectControlPointCommand::~SelectControlPointCommand() {}

void SelectControlPointCommand::Execute() {
    parentArea_->addEditControlPoint(index_, controlPoint_);
}

void SelectControlPointCommand::Undo() {
    parentArea_->removeEditControlPoint(index_);
}

DeselectControlPointCommand::DeselectControlPointCommand(ControlPointEditArea* _parentArea, const Stage::ControlPoint& _controlPoint, int32_t index)
    : parentArea_(_parentArea), controlPoint_(_controlPoint), index_(index) {}

DeselectControlPointCommand::~DeselectControlPointCommand() {}

void DeselectControlPointCommand::Execute() {
    parentArea_->removeEditControlPoint(index_);
}

void DeselectControlPointCommand::Undo() {
    parentArea_->addEditControlPoint(index_, controlPoint_);
}

ClearSelectedControlPointsCommand::ClearSelectedControlPointsCommand(ControlPointEditArea* _parentArea)
    : parentArea_(_parentArea) {}

ClearSelectedControlPointsCommand::~ClearSelectedControlPointsCommand() {}

void ClearSelectedControlPointsCommand::Execute() {
    for (auto& editControlPoint : parentArea_->getEditControlPoints()) {
        removedControlPoints_.push_back(editControlPoint);
    }

    parentArea_->clearEditControlPoints();
}

void ClearSelectedControlPointsCommand::Undo() {
    // Undo時は、すべての制御点を再度選択する
    for (auto& removedControlPoint : removedControlPoints_) {
        parentArea_->addEditControlPoint(removedControlPoint.first, removedControlPoint.second);
        removedControlPoints_.pop_back();
    }
    removedControlPoints_.clear();
}

SelectLinkCommand::SelectLinkCommand(ControlPointEditArea* _parentArea, const Stage::Link& _link, int32_t index)
    : parentArea_(_parentArea), link_(_link), index_(index) {}

SelectLinkCommand::~SelectLinkCommand() {}

void SelectLinkCommand::Execute() {
    parentArea_->addEditLink(index_, link_);
}

void SelectLinkCommand::Undo() {
    parentArea_->removeEditLink(index_);
}

DeselectLinkCommand::DeselectLinkCommand(ControlPointEditArea* _parentArea, const Stage::Link& _link, int32_t index)
    : parentArea_(_parentArea), link_(_link), index_(index) {}

DeselectLinkCommand::~DeselectLinkCommand() {}

void DeselectLinkCommand::Execute() {
    parentArea_->removeEditLink(index_);
}

void DeselectLinkCommand::Undo() {
    parentArea_->addEditLink(index_, link_);
}

ClearSelectedLinksCommand::ClearSelectedLinksCommand(ControlPointEditArea* _parentArea)
    : parentArea_(_parentArea) {}

ClearSelectedLinksCommand::~ClearSelectedLinksCommand() {}

void ClearSelectedLinksCommand::Execute() {
    for (auto& link : parentArea_->getEditLinks()) {
        removedLinks_.push_back(link);
    }
    parentArea_->clearEditLinks();
}

void ClearSelectedLinksCommand::Undo() {
    // Undo時は、すべてのリンクを再度選択する
    for (auto& [index, link] : parentArea_->getEditLinks()) {
        parentArea_->addEditLink(index, link);
    }
    removedLinks_.clear();
}

#pragma endregion "Commands"

#endif // _DEBUG
