#include "SettingGameCameraTarget.h"

/// engine
#include "camera/CameraManager.h"

// component
#include "component/camera/CameraController.h"
#include "component/player/State/PlayerState.h"
#include "component/transform/CameraTransform.h"

void SettingGameCameraTarget::Initialize() {}

void SettingGameCameraTarget::Update() {
    constexpr Vec3f kCameraOffset = Vec3f(0.0f, 2.0f, -5.0f);

    Entity* playerEntity = GetUniqueEntity("Player");
    Entity* cameraEntity = GetUniqueEntity("GameCamera");
    // カメラのターゲットをプレイヤーに設定
    if (cameraEntity) {
        CameraController* cameraController = GetComponent<CameraController>(cameraEntity);
        CameraTransform* cameraTransform   = GetComponent<CameraTransform>(cameraEntity);

        if (cameraController) {
            cameraController->SetFollowTarget(GetComponent<Transform>(playerEntity));
        }

        if (cameraTransform && playerEntity) {
            Transform* playerTransform = GetComponent<Transform>(playerEntity);
            if (playerTransform) {
                playerTransform->UpdateMatrix();
                Vec3f playerPos            = playerTransform->GetWorldTranslate();
                cameraTransform->translate = playerPos + kCameraOffset;
                cameraTransform->UpdateMatrix();
            }
        }

        CameraManager::GetInstance()->SetTransform(*cameraTransform);

        auto playerState = GetComponent<PlayerState>(playerEntity);
        if (playerState) {
            playerState->SetCameraEntityID(cameraEntity->GetID());
        }
    }
}

void SettingGameCameraTarget::Finalize() {}
