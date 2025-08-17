#pragma once

#include "editor/IEditor.h"

/// stl
#include <memory>

/// engine
#include "camera/debugCamera/DebugCamera.h"
#include <scene/Scene.h>

/// ECS
// component
#include "component/Stage.h"

class StageEditorWindow
    : public Editor::Window {
public:
    StageEditorWindow();
    ~StageEditorWindow();

    void Initialize() override;
    // void DrawGui() override;
    void Finalize() override;

private:
    std::unique_ptr<Scene> scene_;

public:
    Scene* getCurrentScene() {
        return scene_.get();
    }
    void changeScene(std::unique_ptr<Scene>&& _newScene) {
        scene_.reset();
        scene_ = std::move(_newScene);
    }
};

class StageViewArea
    : public Editor::Area {
public:
    StageViewArea();
    ~StageViewArea();
    void Initialize() override;

    void Finalize() override;

public:
    class StageViewRegion
        : public Editor::Region {
    public:
        StageViewRegion(StageViewArea* _parent);
        ~StageViewRegion();
        void Initialize() override;
        void DrawGui() override;
        void Finalize() override;

    private:
        void UpdateScene();
        void DrawScene();

    private:
        StageViewArea* parentWindow_ = nullptr;
        std::unique_ptr<DebugCamera> debugCamera_;

        Scene* currentScene_ = nullptr; // 現在のシーンへのポインタ
    };
};

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
    std::vector<std::pair<int32_t, Stage::Link>> editLinks_;

    Vec2f clickedMousePos_;
    Vec2f releasedMousePos_;

public:
    const std::vector<std::pair<int32_t, Stage::ControlPoint>>& getEditControlPoints() const {
        return editControlPoints_;
    }
    const std::vector<std::pair<int32_t, Stage::Link>>& getEditLinks() const {
        return editLinks_;
    }
    std::vector<std::pair<int32_t, Stage::ControlPoint>>& getEditControlPointsRef() {
        return editControlPoints_;
    }
    std::vector<std::pair<int32_t, Stage::Link>>& getEditLinksRef() {
        return editLinks_;
    }

    void addEditControlPoint(int32_t _index, const Stage::ControlPoint& _ctlPoint) {
        editControlPoints_.emplace_back(_index, _ctlPoint);
    }
    void addEditLink(int32_t _index, const Stage::Link& _link) {
        editLinks_.emplace_back(_index, _link);
    }

    void clearEditControlPoints() { editControlPoints_.clear(); }
    void clearEditLinks() { editLinks_.clear(); }

    void removeEditControlPoint(int32_t _index) {
        editControlPoints_.erase(
            std::remove_if(editControlPoints_.begin(), editControlPoints_.end(),
                [_index](const auto& p) { return p.first == _index; }),
            editControlPoints_.end());
    }
    void removeEditLink(int32_t _index) {
        editLinks_.erase(
            std::remove_if(editLinks_.begin(), editLinks_.end(),
                [_index](const auto& p) { return p.first == _index; }),
            editLinks_.end());
    }
};

/// ==========================================
// command
/// ==========================================
#pragma region "Commands"
class AddControlPointCommand
    : public IEditCommand {
public:
    AddControlPointCommand(Stage* stage, const Stage::ControlPoint& point);
    ~AddControlPointCommand();

    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    Stage::ControlPoint point_;
    int32_t addedPointIdx_ = -1; // 追加した制御点のインデックス
};
class RemoveControlPointCommand
    : public IEditCommand {
public:
    RemoveControlPointCommand(Stage* stage, int32_t index);
    ~RemoveControlPointCommand();

    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    int32_t index_;
    Stage::ControlPoint removedPoint_; // 削除した制御点の情報
};
class ClearControlPointsCommand
    : public IEditCommand {
public:
    ClearControlPointsCommand(Stage* stage);

    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    std::vector<Stage::ControlPoint> removedPoints_; // 削除した制御点の情報
};

class AddLinkCommand
    : public IEditCommand {
public:
    AddLinkCommand(Stage* stage, const Stage::Link& link);
    ~AddLinkCommand() override;

    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    int32_t addedLinkIdx_ = -1; // 追加したリンクのインデックス
    Stage::Link link_;
};
class RemoveLinkCommand
    : public IEditCommand {
public:
    RemoveLinkCommand(Stage* stage, int32_t index);
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    int32_t index_;
    Stage::Link removedLink_; // 削除したリンクの情報
};
class ClearLinksCommand
    : public IEditCommand {
public:
    ClearLinksCommand(Stage* stage);
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    std::vector<Stage::Link> removedLinks_; // 削除したリンクの情報
};

class SortControlPointsCommand
    : public IEditCommand {
public:
    SortControlPointsCommand(Stage* _stage, ControlPointEditArea* _parentWindow)
        : stage_(_stage), parentWindow_(_parentWindow) {}
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    ControlPointEditArea* parentWindow_ = nullptr; // 親ウィンドウへのポインタ

    std::vector<std::pair<int32_t, Stage::ControlPoint>> indexedPoints_;
    std::unordered_map<int32_t, int32_t> indexBind_;
};

class SelectControlPointCommand
    : public IEditCommand {
public:
    SelectControlPointCommand(ControlPointEditArea* _parentArea, const Stage::ControlPoint& _ctlPoint, int32_t index);
    ~SelectControlPointCommand() override;

    void Execute() override;
    void Undo() override;

private:
    ControlPointEditArea* parentArea_;
    Stage::ControlPoint controlPoint_;
    int32_t index_;
};
class DeselectControlPointCommand
    : public IEditCommand {
public:
    DeselectControlPointCommand(ControlPointEditArea* _parentArea, const Stage::ControlPoint& _controlPoint, int32_t index);
    ~DeselectControlPointCommand();

    void Execute() override;
    void Undo() override;

private:
    ControlPointEditArea* parentArea_;
    Stage::ControlPoint controlPoint_;
    int32_t index_;
};
class ClearSelectedControlPointsCommand
    : public IEditCommand {
public:
    ClearSelectedControlPointsCommand(ControlPointEditArea* _parentArea);
    ~ClearSelectedControlPointsCommand();
    void Execute() override;
    void Undo() override;

private:
    std::vector<std::pair<int, Stage::ControlPoint>> removedControlPoints_;
    ControlPointEditArea* parentArea_;
};

class SelectLinkCommand
    : public IEditCommand {
public:
    SelectLinkCommand(ControlPointEditArea* _parentArea, const Stage::Link& _link, int32_t index);
    ~SelectLinkCommand();
    void Execute() override;
    void Undo() override;

private:
    ControlPointEditArea* parentArea_;
    Stage::Link link_;
    int32_t index_;
};
class DeselectLinkCommand
    : public IEditCommand {
public:
    DeselectLinkCommand(ControlPointEditArea* _parentArea, const Stage::Link& _link, int32_t index);
    ~DeselectLinkCommand();
    void Execute() override;
    void Undo() override;

private:
    ControlPointEditArea* parentArea_;
    Stage::Link link_;
    int32_t index_;
};
class ClearSelectedLinksCommand
    : public IEditCommand {
public:
    ClearSelectedLinksCommand(ControlPointEditArea* _parentArea);
    ~ClearSelectedLinksCommand() override;

    void Execute() override;
    void Undo() override;

private:
    ControlPointEditArea* parentArea_;
    std::vector<std::pair<int, Stage::Link>> removedLinks_;
};

#pragma endregion "Commands"
