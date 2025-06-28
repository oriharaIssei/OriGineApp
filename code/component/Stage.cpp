#include "Stage.h"

/// stl
#include <unordered_map>

#ifdef DEBUG
#include "myGui/MyGui.h"

/// ==========================================
// command
/// ==========================================
class AddControlPointCommand
    : public IEditCommand {
public:
    AddControlPointCommand(Stage* stage, const Stage::ControlPoint& point)
        : stage_(stage), point_(point) {}

    void Execute() override {
        addedPointIdx_ = static_cast<int32_t>(stage_->getControlPoints().size());
        stage_->addControlPoint(point_);
    }
    void Undo() override {
        stage_->removeControlPoint(addedPointIdx_);
    }

private:
    Stage* stage_;
    Stage::ControlPoint point_;
    int32_t addedPointIdx_ = -1; // 追加した制御点のインデックス
};
class RemoveControlPointCommand
    : public IEditCommand {
public:
    RemoveControlPointCommand(Stage* stage, int32_t index)
        : stage_(stage), index_(index) {}
    void Execute() override {
        removedPoint_ = stage_->getControlPoints()[index_];
        stage_->removeControlPoint(index_);
    }
    void Undo() override {
        stage_->addControlPoint(removedPoint_);
    }

private:
    Stage* stage_;
    int32_t index_;
    Stage::ControlPoint removedPoint_; // 削除した制御点の情報
};
class ClearControlPointsCommand
    : public IEditCommand {
public:
    ClearControlPointsCommand(Stage* stage)
        : stage_(stage) {}
    void Execute() override {
        removedPoints_ = stage_->getControlPoints();
        stage_->clearControlPoints();
    }
    void Undo() override {
        for (const auto& point : removedPoints_) {
            stage_->addControlPoint(point);
        }
    }

private:
    Stage* stage_;
    std::vector<Stage::ControlPoint> removedPoints_; // 削除した制御点の情報
};

class AddLinkCommand
    : public IEditCommand {
public:
    AddLinkCommand(Stage* stage, const Stage::Link& link)
        : stage_(stage), link_(link) {}
    void Execute() override {
        addedLinkIdx_ = static_cast<int32_t>(stage_->getLinks().size());
        stage_->addLink(link_);
    }
    void Undo() override {
        stage_->removeLink(addedLinkIdx_);
    }

private:
    Stage* stage_;
    int32_t addedLinkIdx_ = -1; // 追加したリンクのインデックス
    Stage::Link link_;
};
class RemoveLinkCommand
    : public IEditCommand {
public:
    RemoveLinkCommand(Stage* stage, int32_t index)
        : stage_(stage), index_(index) {}
    void Execute() override {
        removedLink_ = stage_->getLinks()[index_];
        stage_->removeLink(index_);
    }
    void Undo() override {
        stage_->addLink(removedLink_);
    }

private:
    Stage* stage_;
    int32_t index_;
    Stage::Link removedLink_; // 削除したリンクの情報
};
class ClearLinksCommand
    : public IEditCommand {
public:
    ClearLinksCommand(Stage* stage)
        : stage_(stage) {}
    void Execute() override {
        removedLinks_ = stage_->getLinks();
        stage_->clearLinks();
    }
    void Undo() override {
        for (const auto& link : removedLinks_) {
            stage_->addLink(link);
        }
    }

private:
    Stage* stage_;
    std::vector<Stage::Link> removedLinks_; // 削除したリンクの情報
};

class SortControlPointsCommand
    : public IEditCommand {
public:
    SortControlPointsCommand(Stage* stage)
        : stage_(stage) {}
    void Execute() override {
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

        // editControlPoints_ のインデックスを変換
        for (auto& pair : stage_->editControlPoints_) {
            pair.first = indexBind_[pair.first];
        }
    }
    void Undo() override {
        // 元のインデックスに戻す
        for (const auto& pair : indexedPoints_) {
            stage_->controlPoints_[pair.first] = pair.second;
        }
        // links_ のインデックスを元に戻す
        for (auto& link : stage_->links_) {
            link.from_ = indexBind_.at(link.from_);
            link.to_   = indexBind_.at(link.to_);
        }
        // editControlPoints_ のインデックスを元に戻す
        for (auto& pair : stage_->editControlPoints_) {
            pair.first = indexBind_.at(pair.first);
        }
    }

private:
    Stage* stage_;
    std::vector<std::pair<int32_t, Stage::ControlPoint>> indexedPoints_;
    std::unordered_map<int32_t, int32_t> indexBind_;
};

class SelectControlPointCommand
    : public IEditCommand {
public:
    SelectControlPointCommand(Stage* stage, int32_t index)
        : stage_(stage), index_(index) {}

    void Execute() override {
        stage_->addEditControlPoint(index_);
    }
    void Undo() override {
        stage_->removeEditControlPoint(index_);
    }

private:
    Stage* stage_;
    int32_t index_;
};
class DeselectControlPointCommand
    : public IEditCommand {
public:
    DeselectControlPointCommand(Stage* stage, int32_t index)
        : stage_(stage), index_(index) {}

    void Execute() override {
        stage_->removeEditControlPoint(index_);
    }
    void Undo() override {
        stage_->addEditControlPoint(index_);
    }

private:
    Stage* stage_;
    int32_t index_;
};
class ClearSelectedControlPointsCommand
    : public IEditCommand {
public:
    ClearSelectedControlPointsCommand(Stage* stage)
        : stage_(stage) {}
    void Execute() override {

        for (auto& [idx, point] : stage_->getEditControlPoints()) {
            removedControlPoints_.push_back(idx);
        }

        stage_->clearEditControlPoints();
    }
    void Undo() override {
        // Undo時は、すべての制御点を再度選択する
        for (int32_t i = 0; i < static_cast<int32_t>(removedControlPoints_.size()); ++i) {
            stage_->addEditControlPoint(removedControlPoints_.back());
            removedControlPoints_.pop_back();
        }
    }

private:
    std::vector<int> removedControlPoints_;
    Stage* stage_;
};

class SelectLinkCommand
    : public IEditCommand {
public:
    SelectLinkCommand(Stage* stage, int32_t index)
        : stage_(stage), index_(index) {}
    void Execute() override {
        stage_->addEditLink(index_);
    }
    void Undo() override {
        stage_->removeEditLink(index_);
    }

private:
    Stage* stage_;
    int32_t index_;
};
class DeselectLinkCommand
    : public IEditCommand {
public:
    DeselectLinkCommand(Stage* stage, int32_t index)
        : stage_(stage), index_(index) {}
    void Execute() override {
        stage_->removeEditLink(index_);
    }
    void Undo() override {
        stage_->addEditLink(index_);
    }

private:
    Stage* stage_;
    int32_t index_;
};
class ClearSelectedLinksCommand
    : public IEditCommand {
public:
    ClearSelectedLinksCommand(Stage* stage)
        : stage_(stage) {}
    void Execute() override {
        for (auto& [idx, link] : stage_->getEditLinks()) {
            removedLinks_.push_back(idx);
        }
        stage_->clearEditLinks();
    }
    void Undo() override {
        // Undo時は、すべてのリンクを再度選択する
        for (int32_t i = 0; i < static_cast<int32_t>(removedLinks_.size()); ++i) {
            stage_->addEditLink(removedLinks_.back());
            removedLinks_.pop_back();
        }
    }

private:
    std::vector<int> removedLinks_;
    Stage* stage_;
};

#endif // DEBUG

void Stage::Initialize(GameEntity* /*_entity*/) {}

bool Stage::Edit() {
    bool isChanged = false;

#ifdef DEBUG
    if (ImGui::Button("+ ControlPoint")) {
        // 制御点を追加する
        // 制御点の初期位置は(0, 0, 0)とする
        Stage::ControlPoint newPoint;
        auto command = std::make_unique<AddControlPointCommand>(this, newPoint);
        EditorController::getInstance()->pushCommand(std::move(command));
        isChanged = true;
    }
    if (ImGui::Button("+ Link")) {
        // リンクを追加するための制御点が必要
        if (editControlPoints_.size() >= 2) {
            int32_t prevCtlPoint = editControlPoints_.front().first;
            auto commandCombo    = std::make_unique<CommandCombo>();

            // 選択されている 制御点を 順に つなぐ
            for (int32_t i = 0; i < editControlPoints_.size(); ++i) {
                Link newLink;
                newLink.from_ = prevCtlPoint;
                newLink.to_   = editControlPoints_[i].first;

                prevCtlPoint = editControlPoints_[i].first;

                commandCombo->addCommand(std::make_shared<AddLinkCommand>(this, newLink));
            }

            EditorController::getInstance()->pushCommand(std::move(commandCombo));

            isChanged = true;
        }
    }

    ImGui::Separator();
    if (ImGui::TreeNode("Control Points")) {
        if (ImGui::Button("Sort Z")) {
            // 制御点をZ座標でソートするコマンドを追加
            auto command = std::make_unique<SortControlPointsCommand>(this);
            EditorController::getInstance()->pushCommand(std::move(command));
            isChanged = true;
        }

        std::string label = "";
        auto& io          = ImGui::GetIO();
        for (size_t i = 0; i < controlPoints_.size(); ++i) {
            auto& ctlPoint = controlPoints_[i];
            ImGui::PushID(static_cast<int>(i));

            // 選択状態か判定
            bool isSelected = std::any_of(
                editControlPoints_.begin(), editControlPoints_.end(),
                [i](const auto& p) { return p.first == static_cast<int32_t>(i); });

            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 1.0f, 1.0f)); // 選択色
            }

            if (ImGui::Button("X")) {
                // 制御点を削除する
                // 選択リストからも削除
                if (isSelected) {
                    auto deselectCommand = std::make_unique<DeselectControlPointCommand>(this, static_cast<int32_t>(i));
                    EditorController::getInstance()->pushCommand(std::move(deselectCommand));
                }
                auto removeCommand = std::make_unique<RemoveControlPointCommand>(this, static_cast<int32_t>(i));
                EditorController::getInstance()->pushCommand(std::move(removeCommand));

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
                        auto selectCommand = std::make_unique<SelectControlPointCommand>(this, static_cast<int32_t>(i));
                        EditorController::getInstance()->pushCommand(std::move(selectCommand));
                    }
                } else {
                    // Shiftなしなら単独選択
                    auto clearCommand = std::make_unique<ClearSelectedControlPointsCommand>(this);
                    EditorController::getInstance()->pushCommand(std::move(clearCommand));
                    auto selectCommand = std::make_unique<SelectControlPointCommand>(this, static_cast<int32_t>(i));
                    EditorController::getInstance()->pushCommand(std::move(selectCommand));
                }
            }
            if (isSelected) {
                ImGui::PopStyleColor();
            }

            DragGuiVectorCommand(label.c_str(), ctlPoint.pos_, 0.01f);
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Links")) {
        std::string label = "";
        auto& io          = ImGui::GetIO();
        for (size_t i = 0; i < links_.size(); ++i) {
            auto& link = links_[i];
            ImGui::PushID(static_cast<int>(i));

            // 選択状態か判定
            bool isSelected = std::any_of(
                editLinks_.begin(), editLinks_.end(),
                [i](const auto& p) { return p.first == static_cast<int32_t>(i); });

            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 1.0f, 1.0f));
            }

            if (ImGui::Button("X")) {
                links_.erase(links_.begin() + i);
                // 選択リストからも削除
                editLinks_.erase(
                    std::remove_if(editLinks_.begin(), editLinks_.end(),
                        [i](const auto& p) { return p.first == static_cast<int32_t>(i); }),
                    editLinks_.end());
                isChanged = true;
                if (isSelected)
                    ImGui::PopStyleColor();
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
                        auto selectCommand = std::make_unique<SelectLinkCommand>(this, static_cast<int32_t>(i));
                        EditorController::getInstance()->pushCommand(std::move(selectCommand));
                    }
                } else {
                    // Shiftなしなら単独選択
                    auto clearCommand = std::make_unique<ClearSelectedLinksCommand>(this);
                    EditorController::getInstance()->pushCommand(std::move(clearCommand));
                    auto selectCommand = std::make_unique<SelectLinkCommand>(this, static_cast<int32_t>(i));
                    EditorController::getInstance()->pushCommand(std::move(selectCommand));
                }
            }
            if (isSelected) {
                ImGui::PopStyleColor();
            }

            if (DragGuiVectorCommand("Normal", link.normal_, 0.01f)) {
                link.normal_ = link.normal_.normalize();
            }
            DragGuiCommand("Height", link.height_, 0.01f);
            DragGuiCommand("Width", link.width_, 0.01f);
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

#endif

    return isChanged;
}

void Stage::Finalize() {
#ifdef DEBUG
    editControlPoints_.clear();
    editLinks_.clear();
    clickedMousePos_  = Vec2f(0.0f, 0.0f);
    releasedMousePos_ = Vec2f(0.0f, 0.0f);
#endif // DEBUG
    controlPoints_.clear();
    links_.clear();
}
