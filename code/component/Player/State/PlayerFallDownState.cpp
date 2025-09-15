#include "PlayerFallDownState.h"

/// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"
#include "component/Player/PlayerInput.h"

void PlayerFallDownState::Initialize() {}

void PlayerFallDownState::Update(float /*_deltaTime*/) {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* playerInput  = scene_->getComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);
    auto* transform    = scene_->getComponent<Transform>(playerEntity);

    // 入力方向を取得
    Vec2f inputDirection = playerInput->getInputDirection();

    if (inputDirection.lengthSq() > 0.f) {
        // カメラの回転を取得
        GameEntity* gameCamera           = scene_->getUniqueEntity("GameCamera");
        const Quaternion& cameraRotation = scene_->getComponent<CameraTransform>(gameCamera)->rotate;

        // カメラの回転からヨー（y軸回転）だけを抽出
        float cameraYaw              = cameraRotation.ToEulerAngles()[Y]; // y成分のみ
        Quaternion cameraYawRotation = Quaternion::RotateAxisAngle(Vec3f(0.f, 1.f, 0.f), cameraYaw);

        // 入力方向の回転
        float inputAngle         = std::atan2(inputDirection[X], inputDirection[Y]);
        Quaternion inputRotation = Quaternion::RotateAxisAngle(Vec3f(0.f, 1.f, 0.f), inputAngle);

        // y軸のみの回転合成
        Quaternion targetRotation = Quaternion::Normalize(cameraYawRotation * inputRotation);

        // プレイヤーの回転を補間して設定
        transform->rotate = Slerp(transform->rotate, targetRotation, playerStatus->getDirectionInterpolateRate());
    }

    // 移動方向を回転
    // 落下中は 速度が落ちない,止まらない

    Vec3f oldVelo   = rigidbody->getVelocity();
    Vec2f oldXZVelo = {oldVelo[X], oldVelo[Z]};

    Vec3f movementDirection = Vec3f(0.f, 0.f, 1.f) * MakeMatrix::RotateQuaternion(transform->rotate);
    Vec3f velocity          = {0.f, 0.f, 0.f};
    if (oldXZVelo.lengthSq() != 0.f) {
        // 移動方向を回転
        velocity = movementDirection * oldXZVelo.length();
    }

    velocity[Y] = oldVelo[Y]; // 落下中はY軸の速度を保持
    rigidbody->setVelocity(velocity);
}

void PlayerFallDownState::Finalize() {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto* playerStatus = scene_->getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = scene_->getComponent<Rigidbody>(playerEntity);

    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f}); // ダッシュ終了時に加速度をリセット
    Vec3f velo = rigidbody->getVelocity();

    float limitSpeed = playerStatus->getCurrentSpeed(); // このフレームに追加される速度を引く
    if (velo.lengthSq() >= limitSpeed * limitSpeed) {
        // 速度が速すぎる場合は制限
        velo = velo.normalize() * limitSpeed;
        rigidbody->setVelocity(velo);
    }

    auto* skinningAnim = scene_->getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->Play(4); // 差し替え
    }
}

PlayerMoveState PlayerFallDownState::TransitionState() const {
    auto* playerEntity = scene_->getEntity(playerEntityID_);
    auto playerStatus  = scene_->getComponent<PlayerStatus>(playerEntity);
    auto playerInput   = scene_->getComponent<PlayerInput>(playerEntity);

    if (playerStatus->isOnGround()) {
        if (playerStatus->isCollisionWithWall()) {
            return PlayerMoveState::WALL_RUN;
        } else {
            if (playerInput->getInputDirection().lengthSq() > 0.f) {
                return PlayerMoveState::DASH;
            }
        }
        return PlayerMoveState::IDLE;
    }

    return PlayerMoveState::FALL_DOWN;
}
