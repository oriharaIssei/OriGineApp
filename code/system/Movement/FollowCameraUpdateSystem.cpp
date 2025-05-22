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
        // ============= interTarget ============= //
        cameraController->setInterTarget(Lerp(Vec3f(cameraController->getFollowTarget()->worldMat[3]), cameraController->getInterTarget(), cameraController->getInterTargetInterpolation()));

        // ============= rotate ============= //
        // Quaternion で 回転
        Vec2f destinationAngleXY = cameraController->getDestinationAngleXY();

        Quaternion destinationAngleQuat = Quaternion::FromEulerAngles({destinationAngleXY, 0.f});
        cameraTransform->rotate         = Slerp(cameraTransform->rotate, destinationAngleQuat, cameraController->getRotateSensitivity());
        cameraTransform->rotate[X]      = std::clamp(cameraTransform->rotate[X], cameraController->getMinRotateX(), cameraController->getMaxRotateX());

        // ============= translate ============= //
        // FollowTarget を中心に回転
        Vec3f followTargetPosition = cameraController->getInterTarget();
        Vec3f offset               = cameraController->getFollowOffset();

        // 回転行列を適用
        Vec3f rotatedOffset = offset * MakeMatrix::RotateQuaternion(cameraTransform->rotate);

        // 新しいカメラ位置を計算
        cameraTransform->translate = followTargetPosition + rotatedOffset;
    }

    cameraTransform->UpdateMatrix();

    CameraManager::getInstance()->setTransform(*cameraTransform);
}
