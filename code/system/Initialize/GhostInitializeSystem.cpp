#include "GhostInitializeSystem.h"

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

#include "system/effect/CameraShake.h"

/// application
#include "manager/PlayerProgressStore.h"

/// external
#include "logger/Logger.h"

GhostInitializeSystem::GhostInitializeSystem() : ISystem(SystemCategory::Initialize) {}
GhostInitializeSystem::~GhostInitializeSystem() {}

void GhostInitializeSystem::Initialize() {}
void GhostInitializeSystem::Finalize() {}

void GhostInitializeSystem::UpdateEntity(Entity* _entity) {
    auto ghostReplayComp = GetComponent<GhostReplayComponent>(_entity);

    if (!ghostReplayComp) {
        return;
    }

    if (!InitializeGhostReplayComponent(ghostReplayComp)) {
        LOG_ERROR("Failed to initialize GhostReplayComponent.");
        GetScene()->AddDeleteEntity(_entity->GetID()); // 初期化に失敗したらエンティティを削除
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
    if (!stageDataEntity) {
        return false;
    }
    auto stageData = scene->GetComponent<StageData>(stageDataEntity);
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
    SceneSerializer serializer = SceneSerializer(scene);
    auto copyPlayerJson        = serializer.EntityToJson(scene->GetUniqueEntity("Player")->GetID());
    auto copyCameraJson        = serializer.EntityToJson(scene->GetUniqueEntity("GameCamera")->GetID());

    // 名前は変える
    copyPlayerJson["Name"]     = "GhostPlayer";
    copyPlayerJson["isUnique"] = false;
    copyCameraJson["Name"]     = "GhostGameCamera";
    Entity* ghostPlayerEnt     = serializer.EntityFromJson(copyPlayerJson);
    Entity* ghostCameraEnt     = serializer.EntityFromJson(copyCameraJson);

    // どちらも保存しないように設定
    ghostPlayerEnt->SetShouldSave(false);
    ghostCameraEnt->SetShouldSave(false);

    _comp->ghostEntityId_       = ghostPlayerEnt->GetID();
    _comp->ghostCameraEntityId_ = ghostCameraEnt->GetID();

    auto systemRunner = scene->GetSystemRunnerRef();
    // player Input system から ghostを除外しておく
    auto playerInputSystem = systemRunner->GetSystemRef<PlayerInputSystem>();
    if (playerInputSystem) {
        playerInputSystem->RemoveEntity(ghostPlayerEnt);
    }
    // camera Input system から ghostを除外しておく
    auto cameraInputSystem = systemRunner->GetSystemRef<CameraInputSystem>();
    if (cameraInputSystem) {
        cameraInputSystem->RemoveEntity(ghostCameraEnt);
    }
    auto effectOnPlayerRunSystem = systemRunner->GetSystemRef<EffectOnPlayerRun>();
    if (effectOnPlayerRunSystem) {
        effectOnPlayerRunSystem->RemoveEntity(ghostPlayerEnt);
    }
    auto effectOnPlayerGearupSystem = systemRunner->GetSystemRef<EffectOnPlayerGearup>();
    if (effectOnPlayerGearupSystem) {
        effectOnPlayerGearupSystem->RemoveEntity(ghostPlayerEnt);
    }

    auto playerTransform  = GetComponent<Transform>(ghostPlayerEnt);
    auto cameraController = GetComponent<CameraController>(ghostCameraEnt);
    cameraController->SetFollowTarget(playerTransform);

    auto playerState = GetComponent<PlayerState>(ghostPlayerEnt);
    playerState->SetCameraEntityID(ghostCameraEnt->GetID());

    auto playerMaterial = GetComponent<Material>(ghostPlayerEnt);
    if (playerMaterial) {
        // 赤めにする
        playerMaterial->color_[R] = 0.8f;
        playerMaterial->color_[G] = 0.3f;
        playerMaterial->color_[B] = 0.3f;
    }

    auto cameraTransform = GetComponent<CameraTransform>(ghostCameraEnt);
    if (cameraTransform) {
        cameraTransform->canUseMainCamera = false; // メインカメラとして使えないように
    }

    return true;
}
