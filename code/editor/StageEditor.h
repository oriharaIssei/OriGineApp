#pragma once

#ifdef _DEBUG
#include "editor/IEditor.h"

/// stl
#include <memory>

/// engine
#include "camera/debugCamera/DebugCamera.h"
#include <scene/Scene.h>

/// ECS
// component
#include "component/Stage/Stage.h"

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


class StageFileMenu
    : public Editor::Menu {
public:
    StageFileMenu();
    ~StageFileMenu();
    void Initialize() override;
    void Finalize() override;
};
class StageFileSaveMenuItem
    : public Editor::MenuItem {
public:
    StageFileSaveMenuItem();
    ~StageFileSaveMenuItem() override;
    void Initialize() override;
    void DrawGui() override;
    void Finalize() override;

private:
    std::string directory_;
    std::string filename_;
};
class StageFileLoadMenuItem
    : public Editor::MenuItem {
public:
    StageFileLoadMenuItem();
    ~StageFileLoadMenuItem() override;
    void Initialize() override;
    void DrawGui() override;
    void Finalize() override;

private:
    std::string directory_;
    std::string filename_;
};


#endif // _DEBUG
