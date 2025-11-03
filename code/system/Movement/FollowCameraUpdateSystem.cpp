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
    CameraController* cameraController = getComponent<CameraController>(_entity);
    CameraTransform* cameraTransform   = getComponent<CameraTransform>(_entity);
    const float deltaTime              = getMainDeltaTime();

    if (cameraController->getFollowTarget()) {
        // ======== 回転行列 ======== //
        Vec2f destinationAngleXY  = cameraController->getDestinationAngleXY();
        Matrix4x4 cameraRotateMat = MakeMatrix::RotateX(destinationAngleXY[X]) * MakeMatrix::RotateY(destinationAngleXY[Y]);

        // ======== ターゲット追従補間 ======== //
        Vec3f followTargetPosition = Vec3f(cameraController->getFollowTarget()->getWorldTranslate());
        Vec3f interTarget          = cameraController->getInterTarget();
        interTarget                = LerpByDeltaTime(
            interTarget,
            followTargetPosition,
            deltaTime,
            cameraController->getInterTargetInterpolation());
        cameraController->setInterTarget(interTarget);

        // ======== 注視点 (targetOffset) ======== //
        Vec3f targetPosition = interTarget + (cameraController->getCurrentTargetOffset() * cameraRotateMat);

        // ======== カメラ位置 (offset) ======== //
        Vec3f cameraPos            = interTarget + (cameraController->getCurrentOffset() * cameraRotateMat);
        cameraTransform->translate = cameraPos;

        // ======== カメラ回転 ======== //
        Vec3f lookDir           = Vec3f::Normalize(targetPosition - cameraTransform->translate);
        Quaternion targetQuat   = Quaternion::LookAt(lookDir, axisY);
        cameraTransform->rotate = SlerpByDeltaTime(
            cameraTransform->rotate,
            targetQuat,
            deltaTime,
            cameraController->getRotateSensitivity());
    }

    cameraTransform->UpdateMatrix();
    CameraManager::getInstance()->setTransform(*cameraTransform);
    CameraManager::getInstance()->DataConvertToBuffer();
}
