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
        // ============= rotate ============= //
        // Quaternion で 回転
        Vec3f targetDirection   = Vec3f::Normalize(cameraController->getInterTarget() - cameraTransform->translate);
        Vec3f targetDirectionXZ = Vec3f::Normalize(Vec3f(targetDirection[X], 0.0f, targetDirection[Z]));
        float yaw               = atan2(targetDirectionXZ[X], targetDirectionXZ[Z]);
        float pitch             = atan2(targetDirection[Y], Vec3f::Length(Vec3f(targetDirection[X], 0.0f, targetDirection[Z])));
        Quaternion newRotate    = Quaternion::FromEulerAngles(pitch, yaw, 0.0f);
        cameraTransform->rotate = Lerp(cameraTransform->rotate, newRotate, cameraController->getRotateSensitivity());

        
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
