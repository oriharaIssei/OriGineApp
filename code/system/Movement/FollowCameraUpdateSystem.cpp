#include "FollowCameraUpdateSystem.h"

/// engine
#define DELTA_TIME
#define ENGINE_ECS
#include "camera/CameraManager.h"
#include "EngineInclude.h"
// component
#include "component/Camera/CameraController.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

/// math
#include "math/Interpolation.h"
#include <algorithm>
#include <numbers>

void FollowCameraUpdateSystem::Initialize() {}

void FollowCameraUpdateSystem::Finalize() {}

void FollowCameraUpdateSystem::UpdateEntity(Entity* _entity) {
    auto* cameraController = GetComponent<CameraController>(_entity);
    auto* cameraTransform  = GetComponent<CameraTransform>(_entity);
    auto* transform        = GetComponent<Transform>(_entity);

    const float deltaTime = GetMainDeltaTime();

    if (cameraController->GetFollowTarget()) {
        // ======== 回転行列 ======== //
        Vec2f destinationAngleXY  = cameraController->GetDestinationAngleXY();
        Matrix4x4 cameraRotateMat = MakeMatrix4x4::RotateX(destinationAngleXY[X]) * MakeMatrix4x4::RotateY(destinationAngleXY[Y]);

        // ======== ターゲット追従補間 ======== //
        Vec3f followTargetPosition = Vec3f(cameraController->GetFollowTarget()->GetWorldTranslate());
        Vec3f interTarget          = cameraController->GetInterTarget();
        interTarget                = LerpByDeltaTime(
            interTarget,
            followTargetPosition,
            deltaTime,
            cameraController->GetInterTargetInterpolation());
        cameraController->SetInterTarget(interTarget);

        // ======== 注視点 (targetOffset) ======== //
        Vec3f targetPosition = interTarget + (cameraController->GetCurrentTargetOffset() * cameraRotateMat);

        // ======== カメラ位置 (offset) ======== //
        Vec3f cameraPos            = interTarget + (cameraController->GetCurrentOffset() * cameraRotateMat);
        cameraTransform->translate = cameraPos;

        // ======== カメラ回転 ======== //
        Vec3f lookDir           = Vec3f::Normalize(targetPosition - cameraTransform->translate);
        Quaternion targetQuat   = Quaternion::LookAt(lookDir, axisY);
        cameraTransform->rotate = SlerpByDeltaTime(
            cameraTransform->rotate,
            targetQuat,
            deltaTime,
            cameraController->GetRotateSensitivity());

        // transform に同期
        transform->rotate    = cameraTransform->rotate;
        transform->translate = cameraTransform->translate;
        transform->UpdateMatrix();
    }

    cameraTransform->UpdateMatrix();
    CameraManager::GetInstance()->SetTransform(*cameraTransform);
    CameraManager::GetInstance()->DataConvertToBuffer();
}
