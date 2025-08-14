#include "StageEditor.h"

#include <string>

/// engine
// camera
#include "camera/CameraManager.h"

/// ECS
#include "system/ISystem.h"
// comp
#include "component/Stage.h"
// system
#include "system/Initialize/CreateStage.h"
#include "system/render/StageDebugRender.h"

/// editor
#include "editor/EditorController.h"

/// util
#include "nameof.h"

StageEditorWindow::StageEditorWindow() : Editor::Window(nameof<StageEditorWindow>()) {}

StageEditorWindow::~StageEditorWindow() {}

void StageEditorWindow::Initialize() {
    scene_ = std::make_unique<Scene>("StageEditor");

    scene_->InitializeECS();

    /// ========================================================
    // 必要な Component と Systemだけを使用する
    /// ========================================================
    // component
    {
        auto* componentRepo = scene_->getComponentRepositoryRef();
        componentRepo->registerComponentArray(nameof<Stage>());
    }
    // system
    {
        auto* systemRunner = scene_->getSystemRunnerRef();
        systemRunner->registerSystem(nameof<CreateStage>());
        systemRunner->registerSystem(nameof<StageDebugRender>());
    }

    // Editor用 Entity 初期化
    int stageEntityID = scene_->getEntityRepositoryRef()->CreateEntity("Stage", true);
    scene_->addComponent(nameof<Stage>(), stageEntityID, true);
    scene_->registerSystem(nameof<CreateStage>(), 0, true, true);
    scene_->registerSystem(nameof<StageDebugRender>(), 0, true, true);
}

void StageEditorWindow::Finalize() {}

StageViewArea::StageViewArea() : Editor::Area(nameof<StageViewArea>()) {}

StageViewArea::~StageViewArea() {
}

void StageViewArea::Initialize() {
    addRegion(std::make_shared<StageViewRegion>());
}

void StageViewArea::Finalize() {
}

StageViewArea::StageViewRegion::StageViewRegion() : Editor::Region(nameof<StageViewRegion>()) {}

StageViewArea::StageViewRegion::~StageViewRegion() {}

void StageViewArea::StageViewRegion::Initialize() {
    debugCamera_ = std::make_unique<DebugCamera>();
    debugCamera_->Initialize();
}

void StageViewArea::StageViewRegion::DrawGui() {
    DrawScene();
}

void StageViewArea::StageViewRegion::Finalize() {
    if (debugCamera_) {
        debugCamera_->Finalize();
        debugCamera_.reset();
    }
}

void StageViewArea::StageViewRegion::UpdateScene() {
    auto* scene = EditorController::getInstance()->getWindow<StageEditorWindow>()->getCurrentScene();
    if (!scene) {
        LOG_ERROR("StageViewRegion::UpdateScene: No current scene found.");
        return;
    }
    auto* systemRunner = scene->getSystemRunnerRef();
    if (systemRunner) {
        systemRunner->UpdateCategory(SystemCategory::Initialize);
    } else {
        LOG_ERROR("StageViewRegion::UpdateScene: No system runner found in the current scene.");
    }
}

void StageViewArea::StageViewRegion::DrawScene() {
    CameraManager* cameraManager  = CameraManager::getInstance();
    CameraTransform prevTransform = cameraManager->getTransform();

    cameraManager->setTransform(debugCamera_->getCameraTransform());
    cameraManager->DataConvertToBuffer();

    auto* scene = EditorController::getInstance()->getWindow<StageEditorWindow>()->getCurrentScene();
    if (!scene) {
        LOG_ERROR("StageViewRegion::UpdateScene: No current scene found.");
        return;
    }

    // effect systemの更新
    scene->getSystemRunnerRef()->UpdateCategory(SystemCategory::Render);

    cameraManager->setTransform(prevTransform);
}
