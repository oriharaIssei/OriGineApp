#include "StepEditArea.h"

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

StepEditArea::StepEditArea() : Editor::Area(nameof<StepEditArea>()) {}
StepEditArea::~StepEditArea() {}

void StepEditArea::Initialize() {
    addRegion(std::make_shared<StepEditRegion>(this));
}
void StepEditArea::Finalize() {
    Editor::Area::Finalize();
    editSteps_.clear();
}

// ===============================
StepEditArea::StepEditRegion::StepEditRegion(StepEditArea* _parent)
    : Editor::Region(nameof<StepEditRegion>()), parentArea_(_parent) {}

StepEditArea::StepEditRegion::~StepEditRegion() {}

void StepEditArea::StepEditRegion::Initialize() {}

void StepEditArea::StepEditRegion::DrawGui() {
    auto* currentScene = EditorController::getInstance()->getWindow<StageEditorWindow>()->getCurrentScene();
    if (!currentScene) {
        LOG_ERROR("StepEditRegion::DrawGui: No current scene found.");
        return;
    }

    auto* stage         = currentScene->getComponent<Stage>(currentScene->getUniqueEntity("Stage"));
    auto& steps         = stage->getStepsRef();
    auto& controlPoints = stage->getControlPointsRef();

    if (ImGui::Button("+ Step")) {
        // 制御点を指定してStep追加
        if (!controlPoints.empty()) {
            Stage::Step newStep;
            newStep.pointIndex_ = 0;
            auto command        = std::make_unique<AddStepCommand>(stage, newStep);
            EditorController::getInstance()->pushCommand(std::move(command));
        }
    }

    ImGui::Separator();
    if (ImGui::TreeNode("Steps")) {
        std::string label;
        auto& io = ImGui::GetIO();
        for (int32_t i = 0; i < static_cast<int32_t>(steps.size()); ++i) {
            auto& step = steps[i];
            ImGui::PushID(i);

            if (ImGui::Button("X")) {
                auto command = std::make_unique<RemoveStepCommand>(stage, i);
                EditorController::getInstance()->pushCommand(std::move(command));
                ImGui::PopID();
                continue;
            }

            ImGui::SameLine();
            label = "Step " + std::to_string(i);
            ImGui::Text(label.c_str());

            // 編集GUI
            label = "ControlPointIndex##" + std::to_string(i);
            DragGuiCommand(label, step.pointIndex_, 1, 0, (int32_t)controlPoints.size() - 1, "%d");

            label = "Size##" + std::to_string(i);
            DragGuiVectorCommand(label, step.size_, 0.01f);

            label = "Rotation##" + std::to_string(i);
            DragGuiVectorCommand(label, step.rotate_, 0.01f);
            step.rotate_ = step.rotate_.normalize();

            ImGui::Separator();
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}

void StepEditArea::StepEditRegion::Finalize() {}

AddStepCommand::AddStepCommand(Stage* _stage, const Stage::Step& _step) : stage_(_stage), addedStep_(_step) {}
void AddStepCommand::Execute() {
    stage_->addStep(addedStep_);
    addedStepIdx_ = stage_->getSteps().size() - 1;
}
void AddStepCommand::Undo() {
    stage_->removeStep(addedStepIdx_);
}

RemoveStepCommand::RemoveStepCommand(Stage* _stage, int32_t _index) : stage_(_stage), removedIndex_(_index) {}

void RemoveStepCommand::Execute() {
    removedStep_ = stage_->getStepsRef()[removedIndex_];
    stage_->removeStep(removedIndex_);
}

void RemoveStepCommand::Undo() {
    stage_->insertStep(removedIndex_, removedStep_);
}

void ClearStepsCommand::Execute() {
    removedSteps_ = stage_->getSteps();
    stage_->clearSteps();
}

void ClearStepsCommand::Undo() {
    for (auto& step : removedSteps_) {
        stage_->addStep(step);
    }
    removedSteps_.clear();
}

void ClearSelectedStepsCommand::Execute() {
    removedSteps_ = parentArea_->getEditSteps();
    parentArea_->clearEditSteps();
}
void ClearSelectedStepsCommand::Undo() {
    for (auto& [index, step] : removedSteps_) {
        parentArea_->addEditStep(index, step);
    }
    removedSteps_.clear();
}

#endif
