#pragma once

#ifdef _DEBUG

#include "editor/IEditor.h"

/// stl
#include <memory>
#include <vector>

/// ECS
// component
#include "component/Stage/Stage.h"

class StepEditArea : public Editor::Area {
public:
    StepEditArea();
    ~StepEditArea();
    void Initialize() override;
    void Finalize() override;

public:
    class StepEditRegion : public Editor::Region {
    public:
        StepEditRegion(StepEditArea* _parent);
        ~StepEditRegion();
        void Initialize() override;
        void DrawGui() override;
        void Finalize() override;

    private:
        StepEditArea* parentArea_ = nullptr;
    };

private:
    std::vector<std::pair<int32_t, Stage::Step>> editSteps_;

public:
    const auto& getEditSteps() const { return editSteps_; }
    auto& getEditStepsRef() { return editSteps_; }

    void addEditStep(int32_t index, const Stage::Step& step) {
        editSteps_.emplace_back(index, step);
    }
    void removeEditStep(int32_t index) {
        editSteps_.erase(
            std::remove_if(editSteps_.begin(), editSteps_.end(),
                [index](auto& p) { return p.first == index; }),
            editSteps_.end());
    }
    void clearEditSteps() { editSteps_.clear(); }
};

// ===========================================================
// Step Commands
// ===========================================================

class AddStepCommand : public IEditCommand {
public:
    AddStepCommand(Stage* stage, const Stage::Step& step);
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_ = nullptr;
    Stage::Step addedStep_;
    int32_t addedStepIdx_ = -1;
};

class RemoveStepCommand : public IEditCommand {
public:
    RemoveStepCommand(Stage* stage, int32_t index);
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_ = nullptr;
    Stage::Step removedStep_;
    int32_t removedIndex_;
};

// -----------------------------------------------------------

class ClearStepsCommand : public IEditCommand {
public:
    ClearStepsCommand(Stage* stage)
        : stage_(stage) {}

    void Execute() override;

    void Undo() override;

private:
    Stage* stage_ = nullptr;
    std::vector<Stage::Step> removedSteps_;
};

// -----------------------------------------------------------
// 以下は StepEditArea 向け
// -----------------------------------------------------------

class SelectStepCommand : public IEditCommand {
public:
    SelectStepCommand(StepEditArea* parent, const Stage::Step& step, int32_t index)
        : parentArea_(parent), step_(step), index_(index) {}

    void Execute() override { parentArea_->addEditStep(index_, step_); }
    void Undo() override { parentArea_->removeEditStep(index_); }

private:
    StepEditArea* parentArea_;
    Stage::Step step_;
    int32_t index_;
};

// -----------------------------------------------------------

class DeselectStepCommand : public IEditCommand {
public:
    DeselectStepCommand(StepEditArea* parent, const Stage::Step& step, int32_t index)
        : parentArea_(parent), step_(step), index_(index) {}

    void Execute() override { parentArea_->removeEditStep(index_); }
    void Undo() override { parentArea_->addEditStep(index_, step_); }

private:
    StepEditArea* parentArea_;
    Stage::Step step_;
    int32_t index_;
};

// -----------------------------------------------------------

class ClearSelectedStepsCommand : public IEditCommand {
public:
    ClearSelectedStepsCommand(StepEditArea* parent)
        : parentArea_(parent) {}

    void Execute() override;

    void Undo() override;

private:
    StepEditArea* parentArea_;
    std::vector<std::pair<int32_t, Stage::Step>> removedSteps_;
};

#endif
