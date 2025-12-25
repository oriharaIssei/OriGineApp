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

    EntityHandle playerEntityHandle = GetUniqueEntity("Player");
    EntityHandle cameraEntityHandle = GetUniqueEntity("GameCamera");
    // カメラのターゲットをプレイヤーに設定
    CameraController* cameraController = GetComponent<CameraController>(cameraEntityHandle);
    CameraTransform* cameraTransform   = GetComponent<CameraTransform>(cameraEntityHandle);

    if (!cameraController || !cameraTransform) {
        return;
    }
    Transform* playerTransform = GetComponent<OriGine::Transform>(playerEntityHandle);
    cameraController->SetFollowTarget(playerTransform);

    if (playerTransform) {
        playerTransform->UpdateMatrix();
        OriGine::Vec3f playerPos   = playerTransform->GetWorldTranslate();
        cameraTransform->translate = playerPos + kCameraOffset;
        cameraTransform->UpdateMatrix();
    }

    CameraManager::GetInstance()->SetTransform(*cameraTransform);

    auto playerState = GetComponent<PlayerState>(playerEntityHandle);
    if (playerState) {
        playerState->SetCameraEntityHandle(cameraEntityHandle);
    }
}

void SettingGameCameraTarget::Finalize() {}
