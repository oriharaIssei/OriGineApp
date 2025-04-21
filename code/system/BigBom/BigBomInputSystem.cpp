#include "BigBomInputSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/BigBom/BigBomStatus.h"
#include "component/Bom/BomSpawner.h"
#include "component/Player/PlayerStates.h"

#include "engine/EngineInclude.h"
#include <algorithm> // std::clamp
#include <cmath>
#include <numbers>

BigBomInputSystem::BigBomInputSystem()
    : ISystem(SystemType::Input) {}

BigBomInputSystem::~BigBomInputSystem() {}

void BigBomInputSystem::Initialize() {
    isInited_ = false;
    input_    = Input::getInstance();
}

void BigBomInputSystem::Finalize() {}

void BigBomInputSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity)
        return;

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* playerEntity                 = ecsManager->getUniqueEntity("Player");

    if (!playerEntity)
        return;

    PlayerStates* playerStates = getComponent<PlayerStates>(playerEntity);
    Transform* playerTransform = getComponent<Transform>(playerEntity);

    if (!playerStates->GetIsBigBomHaving())
        return;

    Transform* transform       = getComponent<Transform>(_entity);
    BigBomStatus* bigBomStatus = getComponent<BigBomStatus>(_entity);

    if (!transform || !bigBomStatus)
        return;

    ///============================================================
    /// 位置更新：プレイヤー位置 + オフセット
    ///============================================================
    transform->translate = Vec3f(playerTransform->worldMat[3]) + bigBomStatus->GetOffset();

    float deltaTime = Engine::getInstance()->getDeltaTime();
    float moveSpeed = 2.0f * deltaTime; // 速度をdeltaTimeで調整

    // 入力に応じて X軸方向の変更量を決定
    float xMove = 0.0f;

    ///----------------------------------------------------
    ///  keyBorad
    ///----------------------------------------------------
    if (input_->isPressKey(DIK_LEFT)) {
        xMove = -moveSpeed;
    }
    if (input_->isPressKey(DIK_RIGHT)) {
        xMove = moveSpeed;
    }

    /* ///----------------------------------------------------
     ///  JoyStick
     ///----------------------------------------------------
     if (Input::GetInstance()->GetGamepadState(joyState)) {
         if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) {
             xMove = -moveSpeed;
         } else if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
             xMove = moveSpeed;
         }
     }*/

    // X軸方向の変更を適用
    bigBomStatus->IncrementLaunchDirectin(xMove);
    // 正規化処理
    bigBomStatus->LaunchDirectionNormalize();

    // 傾き制限と、常に上向きにする処理
    float maxY = 0.9f; // 上向きを維持する為のY軸の最低値
    float minX = -0.9f; // X軸の傾きの最小値
    float maxX = 0.9f; // X軸の傾きの最大値

    // X軸の制限
    bigBomStatus->SetLaunchDirectionX(std::clamp(bigBomStatus->GetLaunchDirection()[X], minX, maxX));
    // 緩やかに上向きを維持
    bigBomStatus->SetLaunchDirectionY(max(bigBomStatus->GetLaunchDirection()[Y], maxY));
    // 再度正規化
    bigBomStatus->LaunchDirectionNormalize();
    // Z成分を0に設定
    bigBomStatus->SetLaunchDirectionZ(0.0f);

    ///============================================================
    /// スペースキーで発射
    ///============================================================
    if (input_->isTriggerKey(DIK_SPACE)) {
        bigBomStatus->SetIsLaunch(true);
        playerStates->SetIsBigBomHaving(false);
    }
}
