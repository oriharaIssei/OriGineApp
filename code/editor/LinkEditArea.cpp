#include "LinkEditArea.h"

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

LinkEditArea::LinkEditArea() : Editor::Area(nameof<LinkEditArea>()) {}
LinkEditArea::~LinkEditArea() {}

void LinkEditArea::Initialize() {
    addRegion(std::make_shared<LinkEditRegion>(this));
}
void LinkEditArea::Finalize() {
    Editor::Area::Finalize();
    // クリア
    editLinks_.clear();
}

LinkEditArea::LinkEditRegion::LinkEditRegion(LinkEditArea* _parent)
    : Editor::Region(nameof<LinkEditRegion>()), parentArea_(_parent) {}

LinkEditArea::LinkEditRegion::~LinkEditRegion() {}

void LinkEditArea::LinkEditRegion::Initialize() {}
void LinkEditArea::LinkEditRegion::Finalize() {};

void LinkEditArea::LinkEditRegion::DrawGui() {
    auto* currentScene = EditorController::getInstance()->getWindow<StageEditorWindow>()->getCurrentScene();
    if (!currentScene) {
        LOG_ERROR("LinkEditRegion::DrawGui: No current scene found.");
        return;
    }

    auto* stage         = currentScene->getComponent<Stage>(currentScene->getUniqueEntity("Stage"));
    auto& controlPoints = stage->getControlPointsRef();
    auto& links         = stage->getLinksRef();

    if (ImGui::Button("+ Link")) {
        if (controlPoints.size() >= 2) {
            Stage::Link newLink;
            newLink.from_ = 0;
            newLink.to_   = 1;
            auto command  = std::make_unique<AddLinkCommand>(stage, newLink);
            EditorController::getInstance()->pushCommand(std::move(command));
        }
    }

    ImGui::Separator();
    if (ImGui::TreeNode("Links")) {
        std::string label = "";
        auto& io          = ImGui::GetIO();
        for (int32_t i = 0; i < static_cast<int32_t>(links.size()); ++i) {
            auto& link = links[i];
            ImGui::PushID(i);

            bool isSelected = std::any_of(
                parentArea_->getEditLinks().begin(), parentArea_->getEditLinks().end(),
                [i](const auto& p) { return p.first == i; });

            if (isSelected)
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 1.0f, 1.0f));

            if (ImGui::Button("X")) {
                auto command = std::make_unique<RemoveLinkCommand>(stage, i);
                EditorController::getInstance()->pushCommand(std::move(command));
                if (isSelected)
                    ImGui::PopStyleColor();
                ImGui::PopID();
                continue;
            }

            if (isSelected)
                ImGui::PopStyleColor();

            ImGui::SameLine();
            label = "Link " + std::to_string(i);
            if (ImGui::Selectable(label.c_str(), isSelected, 0, ImVec2(0, 0))) {
                if (io.KeyShift) {
                    if (!isSelected) {
                        auto selectCommand = std::make_unique<SelectLinkCommand>(nullptr, link, i);
                        EditorController::getInstance()->pushCommand(std::move(selectCommand));
                    }
                } else {
                    auto clearCommand = std::make_unique<ClearSelectedLinksCommand>(nullptr);
                    EditorController::getInstance()->pushCommand(std::move(clearCommand));
                    auto selectCommand = std::make_unique<SelectLinkCommand>(nullptr, link, i);
                    EditorController::getInstance()->pushCommand(std::move(selectCommand));
                }
            }

            label = "From##" + std::to_string(i);
            DragGuiCommand(label, link.from_, 1, 0, (int32_t)controlPoints.size() - 1, "%d");

            label = "To##" + std::to_string(i);
            DragGuiCommand(label, link.to_, 1, 0, (int32_t)controlPoints.size() - 1, "%d");

            label = "Normal##" + std::to_string(i);
            if (DragGuiVectorCommand(label, link.normal_, 0.01f))
                link.normal_ = link.normal_.normalize();

            DragGuiCommand("Height", link.height_, 0.01f);
            DragGuiCommand("Width", link.width_, 0.01f);
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}

void LinkEditArea::addEditLink(int32_t _index, const Stage::Link& _link) {
    editLinks_.emplace_back(_index, _link);
}

void LinkEditArea::removeEditLink(int32_t _index) {
    editLinks_.erase(
        std::remove_if(editLinks_.begin(), editLinks_.end(),
            [_index](const auto& p) { return p.first == _index; }),
        editLinks_.end());
}

void LinkEditArea::clearEditLinks() {
    editLinks_.clear();
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

SelectLinkCommand::SelectLinkCommand(LinkEditArea* _parentArea, const Stage::Link& _link, int32_t index)
    : parentArea_(_parentArea), link_(_link), index_(index) {}

SelectLinkCommand::~SelectLinkCommand() {}

void SelectLinkCommand::Execute() {
    parentArea_->addEditLink(index_, link_);
}

void SelectLinkCommand::Undo() {
    parentArea_->removeEditLink(index_);
}

DeselectLinkCommand::DeselectLinkCommand(LinkEditArea* _parentArea, const Stage::Link& _link, int32_t index)
    : parentArea_(_parentArea), link_(_link), index_(index) {}

DeselectLinkCommand::~DeselectLinkCommand() {}

void DeselectLinkCommand::Execute() {
    parentArea_->removeEditLink(index_);
}

void DeselectLinkCommand::Undo() {
    parentArea_->addEditLink(index_, link_);
}

ClearSelectedLinksCommand::ClearSelectedLinksCommand(LinkEditArea* _parentArea)
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

#endif
