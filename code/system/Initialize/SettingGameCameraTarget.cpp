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
    Entity* playerEntity = getUniqueEntity("Player");
    Entity* cameraEntity = getUniqueEntity("GameCamera");
    // カメラのターゲットをプレイヤーに設定
    if (cameraEntity) {
        CameraController* cameraController = getComponent<CameraController>(cameraEntity);
        if (cameraController) {
            cameraController->setFollowTarget(getComponent<Transform>(playerEntity));
        }
    }
}

void SettingGameCameraTarget::Finalize() {}
