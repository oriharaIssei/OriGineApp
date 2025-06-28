#include "FollowCameraUpdateSystem.h"

/// engine
#define ENGINE_ECS
#include "camera/CameraManager.h"
#include "EngineInclude.h"
// component
#include "component/cameraController/CameraController.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

/// math
#include <algorithm>
#include <numbers>

void FollowCameraUpdateSystem::Initialize() {
}

void FollowCameraUpdateSystem::Finalize() {
}

void FollowCameraUpdateSystem::UpdateEntity(GameEntity* _entity) {
    CameraController* cameraController = getComponent<CameraController>(_entity);
    CameraTransform* cameraTransform   = getComponent<CameraTransform>(_entity);

    if (cameraController->getFollowTarget()) {
        // ============= interTarget ============= //
        cameraController->setInterTarget(
            Lerp(
                cameraController->getInterTarget(),
                Vec3f(cameraController->getFollowTarget()->worldMat[3]),
                cameraController->getInterTargetInterpolation()));

        // ============= rotate ============= //
        // 角度からオフセットを回転
        Vec2f destinationAngleXY = cameraController->getDestinationAngleXY();
        Vec3f offset             = cameraController->getFollowOffset();

        // 回転行列を作成
        Matrix4x4 rotateMat = MakeMatrix::RotateX(destinationAngleXY[X]) * MakeMatrix::RotateY(destinationAngleXY[Y]);
        Vec3f rotatedOffset = offset * rotateMat;

        // 新しいカメラ位置
        Vec3f followTargetPosition = cameraController->getInterTarget();
        cameraTransform->translate = followTargetPosition + rotatedOffset;

        // ============= look at target ============= //
        // カメラの向きをターゲットに向ける
        Vec3f lookDir               = Vec3f::Normalize(followTargetPosition - cameraTransform->translate);
        Quaternion targetQuaternion = Quaternion::LookAt(lookDir, axisY);

        cameraTransform->rotate = Lerp(cameraTransform->rotate, targetQuaternion, cameraController->getRotateSensitivity());
    }

    cameraTransform->UpdateMatrix();
    CameraManager::getInstance()->setTransform(*cameraTransform);
}
