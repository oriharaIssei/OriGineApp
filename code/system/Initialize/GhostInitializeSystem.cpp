#include "GhostInitializeSystem.h"

/// engine
#include "scene/SceneFactory.h"

/// ECS
// component
#include "component/camera/CameraController.h"
#include "component/ghost/GhostReplayComponent.h"
#include "component/material/Material.h"
#include "component/player/State/PlayerState.h"
#include "component/stage/StageData.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

// system
#include "system/SystemRunner.h"

#include "system/Input/CameraInputSystem.h"
#include "system/input/PlayerInputSystem.h"

#include "system/effect/EffectOnPlayerGearup.h"
#include "system/effect/EffectOnPlayerRun.h"

/// application
#include "manager/PlayerProgressStore.h"

/// external
#include "logger/Logger.h"

using namespace OriGine;

GhostInitializeSystem::GhostInitializeSystem() : ISystem(SystemCategory::Initialize) {}
GhostInitializeSystem::~GhostInitializeSystem() {}

void GhostInitializeSystem::Initialize() {}
void GhostInitializeSystem::Finalize() {}

void GhostInitializeSystem::UpdateEntity(EntityHandle _handle) {
    auto ghostReplayComp = GetComponent<GhostReplayComponent>(_handle);

    if (!ghostReplayComp) {
        return;
    }

    if (!InitializeGhostReplayComponent(ghostReplayComp)) {
        LOG_ERROR("Failed to initialize GhostReplayComponent.");
        GetScene()->AddDeleteEntity(_handle); // 初期化に失敗したらエンティティを削除
    }
}

bool GhostInitializeSystem::InitializeGhostReplayComponent(GhostReplayComponent* _comp) {
    auto scene = GetScene();

    // シーンの確認
    if (!scene) {
        LOG_ERROR("ParentScene is null.");
        return false;
    }

    // stagedataがない時、replayファイルがない時はスキップ。
    auto stageDataEntity = scene->GetUniqueEntity("StageData");
    auto stageData       = scene->GetComponent<StageData>(stageDataEntity);
    if (!stageData) {
        return false;
    }

    // replayのデータ読み込み
    _comp->replayPlayer_ = std::make_shared<ReplayPlayer>();
    bool isLoaded        = PlayerProgressStore::GetInstance()->LoadBestPlayData(stageData->GetStageNumber(), _comp->replayPlayer_.get());
    if (!isLoaded) {
        return false;
    }

    // inputの初期化
    _comp->keyboardInput_ = std::make_shared<KeyboardInput>();
    _comp->mouseInput_    = std::make_shared<MouseInput>();
    _comp->gamepadInput_  = std::make_shared<GamepadInput>();

    // 入力初期化
    _comp->ApplyInput();

    // parentのPlayerとGameCameraを持ってくる
    // Entityの複製と登録
    SceneFactory factory;
    Entity* ghostPlayerEnt = factory.BuildEntityFromTemplate(scene, "Player");
    Entity* ghostCameraEnt = factory.BuildEntityFromTemplate(scene, "GameCamera");

    if (!ghostPlayerEnt || !ghostCameraEnt) {
        return false;
    }

    // どちらも保存しないように設定
    ghostPlayerEnt->SetShouldSave(false);
    ghostCameraEnt->SetShouldSave(false);

    EntityHandle ghostPlayerHandle = ghostPlayerEnt->GetHandle();
    EntityHandle ghostCameraHandle = ghostCameraEnt->GetHandle();

    _comp->ghostEntityId_       = ghostPlayerEnt->GetHandle();
    _comp->ghostCameraEntityId_ = ghostCameraEnt->GetHandle();

    auto systemRunner = scene->GetSystemRunnerRef();
    // player Input system から ghostを除外しておく
    auto playerInputSystem = systemRunner->GetSystemRef<PlayerInputSystem>();
    if (playerInputSystem) {
        playerInputSystem->RemoveEntity(ghostPlayerHandle);
    }
    // camera Input system から ghostを除外しておく
    auto cameraInputSystem = systemRunner->GetSystemRef<CameraInputSystem>();
    if (cameraInputSystem) {
        cameraInputSystem->RemoveEntity(ghostCameraEnt->GetHandle());
    }
    auto effectOnPlayerRunSystem = systemRunner->GetSystemRef<EffectOnPlayerRun>();
    if (effectOnPlayerRunSystem) {
        effectOnPlayerRunSystem->RemoveEntity(ghostPlayerHandle);
    }
    auto effectOnPlayerGearupSystem = systemRunner->GetSystemRef<EffectOnPlayerGearup>();
    if (effectOnPlayerGearupSystem) {
        effectOnPlayerGearupSystem->RemoveEntity(ghostPlayerHandle);
    }

    auto playerTransform  = GetComponent<Transform>(ghostPlayerHandle);
    auto cameraController = GetComponent<CameraController>(ghostCameraHandle);
    cameraController->SetFollowTarget(playerTransform);

    auto playerState = GetComponent<PlayerState>(ghostPlayerHandle);
    playerState->SetCameraEntityHandle(ghostCameraHandle);

    auto& playerMaterials = GetComponents<Material>(ghostPlayerHandle);
    for (auto& material : playerMaterials) {
        // 赤めにする
        material.color_[0] = 0.8f;
        material.color_[1] = 0.3f;
        material.color_[2] = 0.3f;
        material.color_[3] = 0.6f; // 半透明にする
    }

    auto cameraTransform = GetComponent<CameraTransform>(ghostCameraHandle);
    if (cameraTransform) {
        cameraTransform->canUseMainCamera = false; // メインカメラとして使えないように
    }

    return true;
}
