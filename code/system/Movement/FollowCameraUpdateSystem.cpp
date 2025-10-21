#include "FollowCameraUpdateSystem.h"

/// engine
#define ENGINE_ECS
#include "camera/CameraManager.h"
#include "EngineInclude.h"
// component
#include "component/Camera/CameraController.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

/// math
#include <algorithm>
#include <numbers>

void FollowCameraUpdateSystem::Initialize() {}

void FollowCameraUpdateSystem::Finalize() {}

void FollowCameraUpdateSystem::UpdateEntity(Entity* _entity) {
    CameraController* cameraController = getComponent<CameraController>(_entity);
    CameraTransform* cameraTransform   = getComponent<CameraTransform>(_entity);

    if (cameraController->getFollowTarget()) {
        // ============= interTarget ============= //
        Vec3f followTargetPosition = Vec3f(cameraController->getFollowTarget()->worldMat[3]);

        Vec3f interTarget = Vec3f();
        interTarget       = Lerp(
            cameraController->getInterTarget(),
            followTargetPosition + cameraController->getCurrentTargetOffset(),
            cameraController->getInterTargetInterpolation());
        cameraController->setInterTarget(interTarget);

        // ============= rotate ============= //
        // 角度からオフセットを回転
        Vec2f destinationAngleXY = cameraController->getDestinationAngleXY();

        Vec3f followOffset = cameraController->getCurrentOffset();

        // 回転行列を作成
        Matrix4x4 rotateMat = MakeMatrix::RotateX(destinationAngleXY[X]) * MakeMatrix::RotateY(destinationAngleXY[Y]);

        // 新しいカメラ位置
        cameraTransform->translate = interTarget + (followOffset * rotateMat);

        // ============= look at target ============= //
        // カメラの向きをターゲットに向ける
        Vec3f lookDir               = Vec3f::Normalize(interTarget - cameraTransform->translate);
        Quaternion targetQuaternion = Quaternion::LookAt(lookDir, axisY);

        cameraTransform->rotate = Slerp(cameraTransform->rotate, targetQuaternion, cameraController->getRotateSensitivity());
    }

    cameraTransform->UpdateMatrix();
    CameraManager::getInstance()->setTransform(*cameraTransform);
    CameraManager::getInstance()->DataConvertToBuffer();
}
