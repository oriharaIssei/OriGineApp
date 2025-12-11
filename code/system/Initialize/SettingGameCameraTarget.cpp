#include "SettingGameCameraTarget.h"

/// engine
#include "camera/CameraManager.h"

// component
#include "component/camera/CameraController.h"
#include "component/player/State/PlayerState.h"
#include "component/transform/CameraTransform.h"

using namespace OriGine;

void SettingGameCameraTarget::Initialize() {}

void SettingGameCameraTarget::Update() {
    constexpr OriGine::Vec3f kCameraOffset = OriGine::Vec3f(0.0f, 2.0f, -5.0f);

    OriGine::Entity* playerEntity = GetUniqueEntity("Player");
    OriGine::Entity* cameraEntity = GetUniqueEntity("GameCamera");
    // カメラのターゲットをプレイヤーに設定
    if (cameraEntity) {
        CameraController* cameraController = GetComponent<CameraController>(cameraEntity);
        CameraTransform* cameraTransform   = GetComponent<CameraTransform>(cameraEntity);

        if (cameraController) {
            cameraController->SetFollowTarget(GetComponent<OriGine::Transform>(playerEntity));
        }

        if (cameraTransform && playerEntity) {
            Transform* playerTransform = GetComponent<OriGine::Transform>(playerEntity);
            if (playerTransform) {
                playerTransform->UpdateMatrix();
                OriGine::Vec3f playerPos            = playerTransform->GetWorldTranslate();
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
