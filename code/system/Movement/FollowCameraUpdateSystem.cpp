#include "FollowCameraUpdateSystem.h"

/// engine
#define ENGINE_ECS
#include "camera/CameraManager.h"
#include "EngineInclude.h"
// component
#include "component/cameraController/CameraController.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

void FollowCameraUpdateSystem::Initialize() {
}

void FollowCameraUpdateSystem::Finalize() {
}

void FollowCameraUpdateSystem::UpdateEntity(GameEntity* _entity) {
    CameraController* cameraController = getComponent<CameraController>(_entity);
    CameraTransform* cameraTransform   = getComponent<CameraTransform>(_entity);
    if (cameraController->getFollowTarget()) {

        // ============= translate ============= //

        Vec3f offset = cameraController->getFollowOffset() * MakeMatrix::RotateQuaternion(cameraTransform->rotate);

        cameraTransform->translate = offset + cameraController->getInterTarget();

        // ============= rotate ============= //
        Vec3f diff = -offset;
        diff       = diff.normalize();
        // Yaw（水平回転）を計算
        float yaw = std::atan2(diff[X], diff[Z]);

        // Pitch（垂直回転）を計算
        float pitch = std::asin(-diff[Y]);

        // 回転をクォータニオンとして設定
        Quaternion newRotate = Quaternion::RotateAxisAngle({0.0f, 1.0f, 0.0f}, yaw) * Quaternion::RotateAxisAngle({1.0f, 0.0f, 0.0f}, pitch);

        cameraTransform->rotate = Lerp(cameraTransform->rotate, newRotate, cameraController->getRotateSensitivity());
    }

    cameraTransform->UpdateMatrix();

    CameraManager::getInstance()->setTransform(*cameraTransform);
}
