#include "SettingGameCameraTarget.h"

/// engine
#include "camera/CameraManager.h"

// component
#include "component/camera/CameraController.h"
#include "component/transform/CameraTransform.h"

void SettingGameCameraTarget::Initialize() {}

void SettingGameCameraTarget::Update() {
    Entity* playerEntity = GetUniqueEntity("Player");
    Entity* cameraEntity = GetUniqueEntity("GameCamera");
    // カメラのターゲットをプレイヤーに設定
    if (cameraEntity) {
        CameraController* cameraController = GetComponent<CameraController>(cameraEntity);
        if (cameraController) {
            cameraController->SetFollowTarget(GetComponent<Transform>(playerEntity));
        }
        CameraTransform* cameraTransform = GetComponent<CameraTransform>(cameraEntity);
        CameraManager::GetInstance()->SetTransform(*cameraTransform);
    }
}

void SettingGameCameraTarget::Finalize() {}
