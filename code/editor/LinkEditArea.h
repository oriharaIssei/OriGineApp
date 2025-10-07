#pragma once

#ifdef _DEBUG

#include "editor/IEditor.h"

/// stl
#include <memory>
#include <vector>

/// ECS
// component
#include "component/Stage/Stage.h"

class LinkEditArea
    : public Editor::Area {
public:
    LinkEditArea();
    ~LinkEditArea();
    void Initialize() override;
    void Finalize() override;

public:
    class LinkEditRegion : public Editor::Region {
    public:
        LinkEditRegion(LinkEditArea* _parent);
        ~LinkEditRegion();
        void Initialize() override;
        void DrawGui() override;
        void Finalize() override;

    private:
        LinkEditArea* parentArea_ = nullptr;
    };

private:
    std::vector<std::pair<int32_t, Stage::Link>> editLinks_;

public:
    const auto& getEditLinks() const { return editLinks_; }
    auto& getEditLinksRef() { return editLinks_; }

    void addEditLink(int32_t _index, const Stage::Link& _link);
    void removeEditLink(int32_t _index);
    void clearEditLinks();
};

/// =========================================================
// ↓ Commands
/// =========================================================
class AddLinkCommand : public IEditCommand {
public:
    AddLinkCommand(Stage* stage, const Stage::Link& link);
    ~AddLinkCommand();
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    int32_t addedLinkIdx_ = -1;
    Stage::Link link_;
};

class RemoveLinkCommand : public IEditCommand {
public:
    RemoveLinkCommand(Stage* stage, int32_t index);
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    int32_t index_;
    Stage::Link removedLink_;
};

class ClearLinksCommand : public IEditCommand {
public:
    ClearLinksCommand(Stage* stage);
    void Execute() override;
    void Undo() override;

private:
    Stage* stage_;
    std::vector<Stage::Link> removedLinks_;
};

class SelectLinkCommand : public IEditCommand {
public:
    SelectLinkCommand(LinkEditArea* _parentArea, const Stage::Link& _link, int32_t index);
    ~SelectLinkCommand();
    void Execute() override;
    void Undo() override;

private:
    LinkEditArea* parentArea_;
    Stage::Link link_;
    int32_t index_;
};

class DeselectLinkCommand : public IEditCommand {
public:
    DeselectLinkCommand(LinkEditArea* _parentArea, const Stage::Link& _link, int32_t index);
    ~DeselectLinkCommand();
    void Execute() override;
    void Undo() override;

private:
    LinkEditArea* parentArea_;
    Stage::Link link_;
    int32_t index_;
};

class ClearSelectedLinksCommand : public IEditCommand {
public:
    ClearSelectedLinksCommand(LinkEditArea* _parentArea);
    ~ClearSelectedLinksCommand();
    void Execute() override;
    void Undo() override;

private:
    LinkEditArea* parentArea_;
    std::vector<std::pair<int, Stage::Link>> removedLinks_;
};

#endif
