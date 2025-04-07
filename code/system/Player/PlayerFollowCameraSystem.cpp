#include "PlayerFollowCameraSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
// include

// component
#include "camera/CameraManager.h"
#include "component/Player/PlayerStates.h"

#include "engine/EngineInclude.h"


PlayerFollowCameraSystem::PlayerFollowCameraSystem()
    : ISystem(SystemType::Movement) {}

PlayerFollowCameraSystem::~PlayerFollowCameraSystem() {}

void PlayerFollowCameraSystem::Initialize() {
}

void PlayerFollowCameraSystem::Finalize() {}

void PlayerFollowCameraSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

   GetTransformForPlayer(_entity);

    ///============================================================
    /// プレイヤーのピボット位置に追従
    ///============================================================
    Vec3f pivotPosition = pivotTransform_->translate;

    ///============================================================
    /// カメラのオフセットを適用
    ///============================================================
    // カメラのオフセット
    Vec3f cameraOffset = {0.0f, 2.0f, -35.0f};

    // `pivotTransform_` の回転を考慮してカメラのオフセットを適用
    Matrix4x4 pivotRotationMatrix = MakeMatrix::RotateQuaternion(pivotTransform_->rotate);
    Vec3f adjustedOffset          = Multiply(pivotRotationMatrix, cameraOffset);

    // カメラの位置を更新
    cameraTransform_->translate = pivotPosition + adjustedOffset;

    ///============================================================
    /// カメラの回転をプレイヤーの回転に同期
    ///============================================================
    cameraTransform_->rotationQuaternion = pivotTransform_->rotate;
    ///============================================================
    /// カメラの行列を更新
    ///============================================================
    cameraTransform_->UpdateMatrix();
}

void PlayerFollowCameraSystem::GetTransformForPlayer(GameEntity* _entity) {
    if (isInited_) {
        return;
    }

    cameraTransform_ = getComponent<CameraTransform>(_entity);
    entityPlayerStates_ = getComponent<PlayerStates>(_entity);
    
    if (!cameraTransform_ || !entityPlayerStates_) {
        return;
    }

    if (!entityPlayerStates_->GetTransform()) {
        return;
    }
    if (!entityPlayerStates_->GetPivotTransform()) {
        return;
    }

    transform_      = entityPlayerStates_->GetTransform();
    pivotTransform_ = entityPlayerStates_->GetPivotTransform();

    CameraManager::getInstance()->setTransform(*cameraTransform_);

    isInited_ = true;
}

Vec3f PlayerFollowCameraSystem::Multiply(const Matrix4x4& rotationMatrix, const Vec3f& offset) {
    return Vec3f(
        rotationMatrix.m[0][0] * offset[X] + rotationMatrix.m[1][0] * offset[Y] + rotationMatrix.m[2][0] * offset[Z] + rotationMatrix.m[3][0],
        rotationMatrix.m[0][1] * offset[X] + rotationMatrix.m[1][1] * offset[Y] + rotationMatrix.m[2][1] * offset[Z] + rotationMatrix.m[3][1],
        rotationMatrix.m[0][2] * offset[X] + rotationMatrix.m[1][2] * offset[Y] + rotationMatrix.m[2][2] * offset[Z] + rotationMatrix.m[3][2]);
}
