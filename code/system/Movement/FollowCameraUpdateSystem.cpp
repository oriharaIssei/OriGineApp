#include "FollowCameraUpdateSystem.h"

/// stl
#include <algorithm>

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
#include <numbers>

using namespace OriGine;

void FollowCameraUpdateSystem::Initialize() {}

void FollowCameraUpdateSystem::Finalize() {}

void FollowCameraUpdateSystem::UpdateEntity(OriGine::Entity* _entity) {
    auto* cameraController = GetComponent<CameraController>(_entity);
    auto* cameraTransform  = GetComponent<CameraTransform>(_entity);
    auto* transform        = GetComponent<OriGine::Transform>(_entity);

    const float deltaTime = GetMainDeltaTime();

    if (cameraController->GetFollowTarget()) {
        // ======== 回転行列 ======== //
        OriGine::Vec2f destinationAngleXY = cameraController->GetDestinationAngleXY();

        // 自動注視処理
        if (cameraController->GetIsAutoLookAtPlayer()) {
            OriGine::Vec3f toTarget = OriGine::Vec3f::Normalize(
                OriGine::Vec3f(cameraController->GetFollowTarget()->GetWorldTranslate()) - cameraTransform->translate);
            float targetAngleY = std::atan2(toTarget[X], toTarget[Z]);

            float currentY = destinationAngleXY[Y];
            float t        = deltaTime * cameraController->GetInterTargetInterpolation();

            destinationAngleXY[Y] = LerpAngle(currentY, targetAngleY, t);

            cameraController->SetDestinationAngleXY(destinationAngleXY);
        }

        Matrix4x4 cameraRotateMat = MakeMatrix4x4::RotateX(destinationAngleXY[X]) * MakeMatrix4x4::RotateY(destinationAngleXY[Y]);

        // ======== ターゲット追従補間 ======== //
        OriGine::Vec3f followTargetPosition = OriGine::Vec3f(cameraController->GetFollowTarget()->GetWorldTranslate());
        OriGine::Vec3f interTarget          = cameraController->GetInterTarget();
        interTarget                         = LerpByDeltaTime(
            interTarget,
            followTargetPosition,
            deltaTime,
            cameraController->GetInterTargetInterpolation());
        cameraController->SetInterTarget(interTarget);

        // ======== 注視点 (targetOffset) ======== //
        OriGine::Vec3f targetPosition = interTarget + (cameraController->GetCurrentTargetOffset() * cameraRotateMat);

        // ======== カメラ位置 (offset) ======== //
        OriGine::Vec3f cameraPos   = interTarget + (cameraController->GetCurrentOffset() * cameraRotateMat);
        cameraTransform->translate = cameraPos;

        // ======== カメラ回転 ======== //
        OriGine::Vec3f lookDir  = OriGine::Vec3f::Normalize(targetPosition - cameraTransform->translate);
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
