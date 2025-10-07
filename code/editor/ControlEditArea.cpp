#include "ControlEditArea.h"

#ifdef _DEBUG

#include <string>

/// editor
#include "editor/EditorController.h"
#include "editor/StageEditor.h"

/// externals
#include <imgui/imgui.h>

/// util
#include "myFileSystem/MyFileSystem.h"
#include "myGui/MyGui.h"
#include "nameof.h"

ControlPointEditArea::ControlPointEditArea() : Editor::Area(nameof<ControlPointEditArea>()) {}
ControlPointEditArea::~ControlPointEditArea() {}

void ControlPointEditArea::Initialize() {
    addRegion(std::make_shared<ControlPointEditRegion>(this));
}
void ControlPointEditArea::Finalize() {
    Editor::Area::Finalize();
    // クリア
    editControlPoints_.clear();
    clickedMousePos_  = Vec2f(0.f, 0.f);
    releasedMousePos_ = Vec2f(0.f, 0.f);
}

ControlPointEditArea::ControlPointEditRegion::ControlPointEditRegion(ControlPointEditArea* _parent)
    : Editor::Region(nameof<ControlPointEditRegion>()), parentArea_(_parent) {}

ControlPointEditArea::ControlPointEditRegion::~ControlPointEditRegion() {}

void ControlPointEditArea::ControlPointEditRegion::Initialize() {}
void ControlPointEditArea::ControlPointEditRegion::Finalize() {};

void ControlPointEditArea::ControlPointEditRegion::DrawGui() {
    auto* currentScene = EditorController::getInstance()->getWindow<StageEditorWindow>()->getCurrentScene();
    if (!currentScene) {
        LOG_ERROR("ControlPointEditRegion::DrawGui: No current scene found.");
        return;
    }
    auto* stage         = currentScene->getComponent<Stage>(currentScene->getUniqueEntity("Stage"));
    auto& controlPoints = stage->getControlPointsRef();

    if (ImGui::Button("+ ControlPoint")) {
        // 制御点を追加する
        // 制御点の初期位置は(0, 0, 0)とする
        Stage::ControlPoint newPoint;
        auto command = std::make_unique<AddControlPointCommand>(stage, newPoint);
        EditorController::getInstance()->pushCommand(std::move(command));
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
};

void ControlPointEditArea::removeEditControlPoint(int32_t _index) {
    editControlPoints_.erase(
        std::remove_if(editControlPoints_.begin(), editControlPoints_.end(),
            [_index](const auto& p) { return p.first == _index; }),
        editControlPoints_.end());
}

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

void SortControlPointsCommand::Execute() {
    if (!indexedPoints_.empty()) {
        indexedPoints_.clear();
    }
    // ソート前のインデックスと制御点のペアを作成
    auto& controlPoints = stage_->getControlPointsRef();
    for (int32_t i = 0; i < controlPoints.size(); ++i) {
        indexedPoints_.emplace_back(i, controlPoints[i]);
    }

    // Zでソート
    std::sort(indexedPoints_.begin(), indexedPoints_.end(),
        [](const auto& a, const auto& b) {
            return a.second.pos_[Z] < b.second.pos_[Z];
        });

    // ソート後のインデックスバインドを作成（oldIdx -> newIdx）
    for (int32_t newIdx = 0; newIdx < indexedPoints_.size(); ++newIdx) {
        int32_t oldIdx        = indexedPoints_[newIdx].first;
        indexBind_[oldIdx]    = newIdx;
        controlPoints[newIdx] = indexedPoints_[newIdx].second;
    }

    // links_ のインデックスを変換
    auto& links = stage_->getLinksRef();
    for (auto& link : links) {
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
    auto& controlPoints = stage_->getControlPointsRef();
    for (const auto& pair : indexedPoints_) {
        controlPoints[pair.first] = pair.second;
    }
    // links_ のインデックスを元に戻す
    auto& links = stage_->getLinksRef();
    for (auto& link : links) {
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

#endif
