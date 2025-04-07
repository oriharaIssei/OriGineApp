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
    CameraManager::getInstance()->setTransform(*cameraTransform_);

    ///============================================================
    /// プレイヤーのピボット位置を取得
    ///============================================================
    Vec3f pivotPosition = pivotTransform_->translate;

    ///============================================================
    /// カメラのオフセットを適用
    ///============================================================
    // カメラのオフセット（前方から見る場合、Z値を正にする）
    Vec3f cameraOffset = {0.0f, 8.0f, 56.0f}; // 変更: Z を +30 に

    // `pivotTransform_` の回転を考慮してカメラのオフセットを適用
    Matrix4x4 pivotRotationMatrix = MakeMatrix::RotateQuaternion(pivotTransform_->rotate);
    Vec3f adjustedOffset          = Multiply(pivotRotationMatrix, cameraOffset);

    // カメラの位置を更新
    Vec3f cameraPosition        = pivotPosition + adjustedOffset;
    cameraTransform_->translate = cameraPosition;

    ///============================================================
    /// カメラの回転をプレイヤーに向ける
    ///============================================================
    // ピボット方向を向くクォータニオンを計算
    Vec3f direction         = Vec3f::Normalize(pivotPosition - cameraPosition); // カメラ → ピボット方向ベクトル
    Quaternion lookRotation = LookAt(direction, Vec3f{0.0f, 1.0f, 0.0f});

    // カメラの回転を設定
    cameraTransform_->rotationQuaternion = lookRotation;

    ///============================================================
    /// カメラの行列を更新
    ///============================================================
    cameraTransform_->UpdateMatrixQuaterion();
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

   

    isInited_ = true;
}

Vec3f PlayerFollowCameraSystem::Multiply(const Matrix4x4& rotationMatrix, const Vec3f& offset) {
    return Vec3f(
        rotationMatrix.m[0][0] * offset[X] + rotationMatrix.m[1][0] * offset[Y] + rotationMatrix.m[2][0] * offset[Z] + rotationMatrix.m[3][0],
        rotationMatrix.m[0][1] * offset[X] + rotationMatrix.m[1][1] * offset[Y] + rotationMatrix.m[2][1] * offset[Z] + rotationMatrix.m[3][1],
        rotationMatrix.m[0][2] * offset[X] + rotationMatrix.m[1][2] * offset[Y] + rotationMatrix.m[2][2] * offset[Z] + rotationMatrix.m[3][2]);
}

Quaternion PlayerFollowCameraSystem::LookAt(const Vec3f& direction, const Vec3f& up) {
    // Z軸を向けるべき方向にする
    Vec3f forward = Vec3f::Normalize(direction);

    // 右ベクトルを計算（外積）
    Vec3f right = Vec3f::Normalize(Vec3f::Cross(up, forward));

    // 上ベクトルを再計算
    Vec3f newUp = Vec3f::Cross(forward, right);

    // 回転行列を作成
    Matrix4x4 lookAtMatrix = {
        right[X], newUp[X], forward[X], 0.0f,
        right[Y], newUp[Y], forward[Y], 0.0f,
        right[Z], newUp[Z], forward[Z], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    // 行列からクォータニオンに変換
    return FromMatrix(lookAtMatrix);
}

Quaternion PlayerFollowCameraSystem::FromMatrix(const Matrix4x4& matrix) {
    float trace = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];

    if (trace > 0.0f) {
        float s    = std::sqrt(trace + 1.0f) * 2.0f;
        float invS = 1.0f / s;

        return Quaternion(
            (matrix.m[2][1] - matrix.m[1][2]) * invS, // x
            (matrix.m[0][2] - matrix.m[2][0]) * invS, // y
            (matrix.m[1][0] - matrix.m[0][1]) * invS, // z
            0.25f * s // w
        );
    } else {
        if (matrix.m[0][0] > matrix.m[1][1] && matrix.m[0][0] > matrix.m[2][2]) {
            float s    = std::sqrt(1.0f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2]) * 2.0f;
            float invS = 1.0f / s;

            return Quaternion(
                0.25f * s,
                (matrix.m[0][1] + matrix.m[1][0]) * invS,
                (matrix.m[0][2] + matrix.m[2][0]) * invS,
                (matrix.m[2][1] - matrix.m[1][2]) * invS);
        } else if (matrix.m[1][1] > matrix.m[2][2]) {
            float s    = std::sqrt(1.0f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][2]) * 2.0f;
            float invS = 1.0f / s;

            return Quaternion(
                (matrix.m[0][1] + matrix.m[1][0]) * invS,
                0.25f * s,
                (matrix.m[1][2] + matrix.m[2][1]) * invS,
                (matrix.m[0][2] - matrix.m[2][0]) * invS);
        } else {
            float s    = std::sqrt(1.0f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1]) * 2.0f;
            float invS = 1.0f / s;

            return Quaternion(
                (matrix.m[0][2] + matrix.m[2][0]) * invS,
                (matrix.m[1][2] + matrix.m[2][1]) * invS,
                0.25f * s,
                (matrix.m[1][0] - matrix.m[0][1]) * invS);
        }
    }
}
