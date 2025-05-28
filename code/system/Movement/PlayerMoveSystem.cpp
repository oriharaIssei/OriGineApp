#include "PlayerMoveSystem.h"

/// engine
#define ENGINE_COMPONENTS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"

void PlayerMoveSystem::Initialize() {
}

void PlayerMoveSystem::Finalize() {
}

void PlayerMoveSystem::UpdateEntity(GameEntity* _entity) {
    PlayerInput* playerInput   = getComponent<PlayerInput>(_entity);
    PlayerStatus* playerStatus = getComponent<PlayerStatus>(_entity);
    Rigidbody* rigidbody       = getComponent<Rigidbody>(_entity);
    Transform* transform       = getComponent<Transform>(_entity);

    if (playerInput->getInputDirection().lengthSq() <= 0.f) {
        rigidbody->setVelocity({0.f, 0.f, 0.f});
        return;
    }
    const float deltaTime = getMainDeltaTime();

    // 入力方向を取得
    Vec2f inputDirection     = playerInput->getInputDirection();
    Quaternion inputRotation = Quaternion::RotateAxisAngle(Vec3f(0.f, 1.f, 0.f), std::atan2(-inputDirection[Y], inputDirection[X]));
    Vec3f movementDirection  = Vec3f(0.f, 0.f, 1.f);

    // カメラの回転を取得
    GameEntity* gameCamera    = getUniqueEntity("GameCamera");
    Quaternion cameraRotation = getComponent<CameraTransform>(gameCamera)->rotate;

    // 入力方向をカメラ基準に変換
    cameraRotation = Quaternion::Normalize(cameraRotation * inputRotation);

    movementDirection = movementDirection * MakeMatrix::RotateQuaternion(cameraRotation);

    // プレイヤーの回転を補間して設定
    transform->rotate = Slerp(transform->rotate, cameraRotation, playerStatus->getDirectionInterpolateRate());

    // 移動速度の更新
    float playerSpeed = playerStatus->getBaseSpeed();
    rigidbody->setVelocity((playerSpeed * deltaTime) * movementDirection);
}
