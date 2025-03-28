#include "PlayerInput.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
// include
#include "component/Field/FieldStates.h"
#include "component/Player/PlayerStates.h"
#include "engine/EngineInclude.h"
#include<numbers>
#include <iostream>

PlayerInputSystem::PlayerInputSystem()
    : ISystem(SystemType::Input) {}

PlayerInputSystem::~PlayerInputSystem() {}

void PlayerInputSystem::Initialize() {
    input_ = Input::getInstance();
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
    PlayerStates* entityPlayerStates = getComponent<PlayerStates>(_entity);
    if (!entityPlayerStates) {
        return;
    }

    ///============================================================
    // 円運動の更新
    ///============================================================
    float angularSpeed = 0.0f;

    if (input_->isPressKey(DIK_D)) {
        angularSpeed += entityPlayerStates->GetMoveSpeed(); // 反時計回り
    } else if (input_->isPressKey(DIK_A)) {
        angularSpeed -= entityPlayerStates->GetMoveSpeed(); // 時計回り
    }

    // 角度を更新
    float deltaTime = Engine::getInstance()->getDeltaTime();
    float newTheta  = entityPlayerStates->GetTheta() + (angularSpeed * deltaTime);

    // 角度を 0 ~ 2π の範囲に収める
    newTheta = std::fmod(newTheta, 2.0f * std::numbers::pi_v<float>);
    entityPlayerStates->SetTheta(newTheta);


    // 半径に基づいた新しい移動ベクトルを計算
    float moveRadius = entityPlayerStates->GetMoveRadius();

    Vec3f moveDir3d = {
        moveRadius * std::cos(newTheta),
        0.0f,
        moveRadius * std::sin(newTheta)};

   
    // Rigidbody に適用
    entityRigidbody->setVelocity(moveDir3d);
}
