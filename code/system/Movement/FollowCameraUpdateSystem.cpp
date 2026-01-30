#include "FollowCameraUpdateSystem.h"

/// stl
#include <algorithm>

/// engine
#include "Engine.h"

#include "camera/CameraManager.h"
// component
#include "component/Camera/CameraController.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

/// math
#include "math/Interpolation.h"
#include <numbers>

using namespace OriGine;

void FollowCameraUpdateSystem::Initialize() {}

void FollowCameraUpdateSystem::Finalize() {}

void FollowCameraUpdateSystem::UpdateEntity(EntityHandle _handle) {
    auto* cameraController = GetComponent<CameraController>(_handle);
    auto* cameraTransform  = GetComponent<CameraTransform>(_handle);
    auto* transform        = GetComponent<Transform>(_handle);

    const float deltaTime = Engine::GetInstance()->GetDeltaTimer()->GetScaledDeltaTime("Camera");

    if (cameraController->followTarget) {
        // 自動注視処理
        if (cameraController->isAutoLookAtPlayer) {
            Vec3f toTarget     = Vec3f::Normalize(cameraController->followTarget->GetWorldTranslate() - cameraTransform->translate);
            float targetAngleY = std::atan2(toTarget[X], toTarget[Z]);

            float currentY                          = cameraController->destinationAngleXY[Y];
            cameraController->destinationAngleXY[Y] = LerpAngleByDeltaTime(currentY, targetAngleY, deltaTime, cameraController->interTargetInterpolation);
        }

        Matrix4x4 cameraRotateMat = MakeMatrix4x4::RotateX(cameraController->destinationAngleXY[X]) * MakeMatrix4x4::RotateY(cameraController->destinationAngleXY[Y]);

        // ======== ターゲット追従補間 ======== //
        Vec3f followTargetPosition = Vec3f(cameraController->followTarget->GetWorldTranslate());

        cameraController->interTarget = LerpByDeltaTime(
            cameraController->interTarget,
            followTargetPosition,
            deltaTime,
            cameraController->interTargetInterpolation);

        // ======== 注視点 (targetOffset) ======== //
        Vec3f targetPosition = cameraController->interTarget + (cameraController->currentTargetOffset * cameraRotateMat);

        // ======== カメラ位置 (offset) ======== //
        Vec3f cameraPos            = cameraController->interTarget + (cameraController->currentOffset * cameraRotateMat);
        cameraTransform->translate = cameraPos;

        // ======== カメラ回転 ======== //
        Vec3f lookDir                = Vec3f::Normalize(targetPosition - cameraTransform->translate);
        Quaternion targetQuat        = Quaternion::LookAt(lookDir, axisY);
        cameraController->baseRotate = SlerpByDeltaTime(
            cameraController->baseRotate,
            targetQuat,
            deltaTime,
            cameraController->rotateSensitivity)
                                           .normalize();
        cameraTransform->rotate = cameraController->baseRotate * Quaternion::RotateAxisAngle(axisZ, cameraController->currentRotateZ);

        // transform に同期
        transform->rotate    = cameraTransform->rotate;
        transform->translate = cameraTransform->translate;
        transform->UpdateMatrix();
    }

    cameraTransform->UpdateMatrix();
    CameraManager::GetInstance()->SetTransform(GetScene(), *cameraTransform);
    CameraManager::GetInstance()->DataConvertToBuffer(GetScene());
}
