#pragma once

#include "editor/IEditor.h"

/// stl
#include <memory>

/// engine
#include "camera/debugCamera/DebugCamera.h"
#include <scene/Scene.h>

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
        StageViewRegion();
        ~StageViewRegion();
        void Initialize() override;
        void DrawGui() override;
        void Finalize() override;

    private:
        void UpdateScene();
        void DrawScene();

    private:
        std::unique_ptr<DebugCamera> debugCamera_;
    };
};
