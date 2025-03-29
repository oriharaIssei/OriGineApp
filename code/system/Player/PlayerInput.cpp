#include "PlayerInput.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
// include

#include "engine/EngineInclude.h"
#include <cmath>
#include <numbers>

PlayerInputSystem::PlayerInputSystem()
    : ISystem(SystemType::Input) {}

PlayerInputSystem::~PlayerInputSystem() {}

void PlayerInputSystem::Initialize() {
    isInited_ = false;
    input_    = Input::getInstance();
}

void PlayerInputSystem::Finalize() {}

void PlayerInputSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    ///============================================================
    // 必要なコンポーネントを取得
    ///============================================================
    Rigidbody* entityRigidbody = getComponent<Rigidbody>(_entity);
    if (!entityRigidbody) {
        return;
    }

    entityPlayerStates = getComponent<PlayerStates>(_entity);
    if (!entityPlayerStates) {
        return;
    }

    TransformInit(_entity);

    ///============================================================
    // 円運動の更新（Quaternionベース）
    ///============================================================
    float deltaTime      = Engine::getInstance()->getDeltaTime();
    float moveSpeed      = entityPlayerStates->GetMoveSpeed();
    float inputDirection = 0.0f;

    /// 入力で向き決定
    if (input_->isPressKey(DIK_D)) {
        inputDirection += 1.0f; // 反時計回り
       
    }
    if (input_->isPressKey(DIK_A)) {
        inputDirection -= 1.0f; // 時計回り
    }

    // 移動方向をセット
    if (inputDirection != 0.0f) {
        entityPlayerStates->SetDirection(inputDirection);
    }
    ///============================================================
    // Y軸回転のQuaternionを作成
    ///============================================================
    Quaternion rotateAxisY = Quaternion::RotateAxisAngle(Vec3f(0.0f, 1.0f, 0.0f),
        inputDirection * moveSpeed * deltaTime);

    ///============================================================
    // 変換後の位置を計算
    ///============================================================

    // 位置を適用
    pivotTransform_->rotate *= rotateAxisY;
    transform_->rotate = Quaternion::RotateAxisAngle({0.0f, 1.0f, 0.0f}, std::atan2(-entityPlayerStates->GetDirection(), 0.0f));

    /// 更新
    pivotTransform_->Update();
    transform_->Update();
}

void PlayerInputSystem::TransformInit(GameEntity* _entity) {
    if (isInited_) {
        return;
    }

    /// TransformをSet
    entityPlayerStates->SetTransform(getComponent<Transform>(_entity, 0));
    entityPlayerStates->SetPivotTransform(getComponent<Transform>(_entity, 1));

    if (!entityPlayerStates->GetTransform()) {
        return;
    }
    if (!entityPlayerStates->GetPivotTransform()) {
        return;
    }

    transform_      = entityPlayerStates->GetTransform();
    pivotTransform_ = entityPlayerStates->GetPivotTransform();
    ///============================================================
    // ピボットをワールド座標の(0,0,0)に設定
    ///============================================================
    pivotTransform_->translate = Vec3f(0.0f, 0.0f, 0.0f);
    pivotTransform_->rotate    = Quaternion::Identity();
    transform_->rotate         = Quaternion::Identity();
    transform_->parent         = pivotTransform_;

    ///============================================================
    // Transformの初期位置を設定
    ///============================================================
    float moveRadius      = entityPlayerStates->GetMoveRadius();
    transform_->translate = Vec3f(0.0f, 0.0f, moveRadius); // X軸上に配置

    ///============================================================
    // Y軸回転のQuaternionを作成
    ///============================================================
    Quaternion rotation     = Quaternion::RotateAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), 0.0f);
    pivotTransform_->rotate *= rotation;

    pivotTransform_->Update();
    transform_->Update();

    isInited_ = true;
}

