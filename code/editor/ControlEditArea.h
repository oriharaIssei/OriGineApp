#pragma once

#ifdef _DEBUG
#include "editor/IEditor.h"

/// stl
#include <memory>

/// ECS
// component
#include "component/Stage/Stage.h"

class ControlPointEditArea
    : public Editor::Area {
public:
    ControlPointEditArea();
    ~ControlPointEditArea();
    void Initialize() override;
    void Finalize() override;

public:
    class ControlPointEditRegion
        : public Editor::Region {

    public:
        ControlPointEditRegion(ControlPointEditArea* _parent);
        ~ControlPointEditRegion();
        void Initialize() override;
        void DrawGui() override;
        void Finalize() override;

    private:
        ControlPointEditArea* parentArea_ = nullptr; // 親ウィンドウへのポインタ
    };

private:
    /// <summary>
    /// first = control point index , second = ControlPoint
    /// </summary>
    std::vector<std::pair<int32_t, Stage::ControlPoint>> editControlPoints_;

    Vec2f clickedMousePos_;
    Vec2f releasedMousePos_;

public:
    const std::vector<std::pair<int32_t, Stage::ControlPoint>>& getEditControlPoints() const {
        return editControlPoints_;
    }
    std::vector<std::pair<int32_t, Stage::ControlPoint>>& getEditControlPointsRef() {
        return editControlPoints_;
    }

    void addEditControlPoint(int32_t _index, const Stage::ControlPoint& _ctlPoint) {
        editControlPoints_.emplace_back(_index, _ctlPoint);
    }
    void clearEditControlPoints() { editControlPoints_.clear(); }

    void removeEditControlPoint(int32_t _index);
};

/// =========================================================
// ↓ Commands
/// =========================================================
class AddControlPointCommand : public IEditCommand {
public:
    AddControlPointCommand(Stage* stage, const Stage::ControlPoint& point);
    ~AddControlPointCommand();
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    Stage::ControlPoint point_;
    int32_t addedPointIdx_ = -1;
};

class RemoveControlPointCommand : public IEditCommand {
public:
    RemoveControlPointCommand(Stage* stage, int32_t index);
    ~RemoveControlPointCommand();
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    int32_t index_;
    std::vector<std::pair<int32_t, bool>> hasControlPointLinksIndex_;
    Stage::ControlPoint removedPoint_;
};

class ClearControlPointsCommand : public IEditCommand {
public:
    ClearControlPointsCommand(Stage* stage);
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    std::vector<Stage::ControlPoint> removedPoints_;
};

class SortControlPointsCommand : public IEditCommand {
public:
    SortControlPointsCommand(Stage* _stage, ControlPointEditArea* _parentWindow)
        : stage_(_stage), parentWindow_(_parentWindow) {}
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    ControlPointEditArea* parentWindow_ = nullptr;
    std::vector<std::pair<int32_t, Stage::ControlPoint>> indexedPoints_;
    std::unordered_map<int32_t, int32_t> indexBind_;
};

class SelectControlPointCommand : public IEditCommand {
public:
    SelectControlPointCommand(ControlPointEditArea* _parentArea, const Stage::ControlPoint& _ctlPoint, int32_t index);
    ~SelectControlPointCommand();
    void Execute() override;
    void Undo() override;

private:
    ControlPointEditArea* parentArea_;
    Stage::ControlPoint controlPoint_;
    int32_t index_;
};

class DeselectControlPointCommand : public IEditCommand {
public:
    DeselectControlPointCommand(ControlPointEditArea* _parentArea, const Stage::ControlPoint& _ctlPoint, int32_t index);
    ~DeselectControlPointCommand();
    void Execute() override;
    void Undo() override;

private:
    ControlPointEditArea* parentArea_;
    Stage::ControlPoint controlPoint_;
    int32_t index_;
};

class ClearSelectedControlPointsCommand : public IEditCommand {
public:
    ClearSelectedControlPointsCommand(ControlPointEditArea* _parentArea);
    ~ClearSelectedControlPointsCommand();
    void Execute() override;
    void Undo() override;

private:
    std::vector<std::pair<int, Stage::ControlPoint>> removedControlPoints_;
    ControlPointEditArea* parentArea_;
};

#endif
