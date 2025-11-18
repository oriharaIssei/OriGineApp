#include "SettingGameCameraTarget.h"

/// engine
#define ENGINE_ECS
#include "EngineInclude.h"

/// app
// component
#include "component/Camera/CameraController.h"

void SettingGameCameraTarget::Initialize() {
}

void SettingGameCameraTarget::Update() {
    Entity* playerEntity = GetUniqueEntity("Player");
    Entity* cameraEntity = GetUniqueEntity("GameCamera");
    // カメラのターゲットをプレイヤーに設定
    if (cameraEntity) {
        CameraController* cameraController = GetComponent<CameraController>(cameraEntity);
        if (cameraController) {
            cameraController->SetFollowTarget(GetComponent<Transform>(playerEntity));
        }
    }
}

void SettingGameCameraTarget::Finalize() {}
