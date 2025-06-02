#include "BigBomInputSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
// component
#include "component/BigBom/BigBomStatus.h"
#include "component/Player/PlayerStates.h"

#include "engine/EngineInclude.h"
#include <algorithm> // std::clamp


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
   /* Transform* childTransform = getComponent<Transform>(playerEntity,0);*/

    if (!playerStates->GetIsBigBomHaving())
        return;

    Transform* transform       = getComponent<Transform>(_entity);
    BigBomStatus* bigBomStatus = getComponent<BigBomStatus>(_entity);

    if (!transform || !bigBomStatus)
        return;

     // スペースで死亡
    if (input_->isTriggerKey(DIK_SPACE)) {
        if (bigBomStatus->GetIsLaunch()) {
            bigBomStatus->SetIsExplotion(true);
        }
    }

    if (bigBomStatus->GetIsLaunch()) {
        return;
    }

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

   //// LaunchDirection に基づいて回転を設定する処理
   // if (bigBomStatus->GetLaunchDirection().lengthSq() > 0.0f) {
   //     Vec3f forward = bigBomStatus->GetLaunchDirection(); // 進行方向
   //     Vec3f up      = Vec3f(0.0f, 1.0f, 0.0f); // ワールドの上方向

   //     // LookRotationで基本の向きを作る
   //     Quaternion baseRotation = LookRotation(forward, up);

   //     // Z軸まわりのロール角（例として時間によって回転）
   //     float time      = Engine::getInstance()->getDeltaTime(); // 経過時間を取得（適宜関数は差し替えてください）
   //     float rollAngle = time * 2.0f * std::numbers::pi_v<float>; // 1秒で1回転の例（必要に応じて変更）

   //     // Z軸回転のクォータニオンを生成
   //     Quaternion roll = Quaternion::RotateAxisAngle(Vec3f(0.0f, 0.0f, 1.0f), rollAngle);

   //     // Z軸回転を合成（先に方向を向いて、その後Z軸ロール）
   //     transform->rotate = baseRotation * roll;
   // }


    ///============================================================
    /// スペースキーで発射
    ///============================================================
    ///

    if (input_->isTriggerKey(DIK_SPACE)) {
        bigBomStatus->SetIsLaunch(true);
    }

    
}


Quaternion BigBomInputSystem::LookRotation(const Vec3f& forward, const Vec3f& up) {
    Vec3f f = forward.normalize();
    Vec3f r = up.cross(f).normalize(); // 右ベクトル
    Vec3f u = f.cross(r); // 補正された上ベクトル

    // 回転行列をクォータニオンに変換
    float m00 = r[X], m01 = u[X], m02 = f[X];
    float m10 = r[Y], m11 = u[Y], m12 = f[Y];
    float m20 = r[Z], m21 = u[Z], m22 = f[Z];

    float trace = m00 + m11 + m22;
    Quaternion q;

    if (trace > 0.0f) {
        float s = 0.5f / std::sqrt(trace + 1.0f);
        q[W]     = 0.25f / s;
        q[X]     = (m21 - m12) * s;
        q[Y]     = (m02 - m20) * s;
        q[Z]     = (m10 - m01) * s;
    } else if (m00 > m11 && m00 > m22) {
        float s = 2.0f * std::sqrt(1.0f + m00 - m11 - m22);
        q[W]     = (m21 - m12) / s;
        q[X]     = 0.25f * s;
        q[Y]     = (m01 + m10) / s;
        q[Z]     = (m02 + m20) / s;
    } else if (m11 > m22) {
        float s = 2.0f * std::sqrt(1.0f + m11 - m00 - m22);
        q[W]     = (m02 - m20) / s;
        q[X]     = (m01 + m10) / s;
        q[Y]     = 0.25f * s;
        q[Z]     = (m12 + m21) / s;
    } else {
        float s = 2.0f * std::sqrt(1.0f + m22 - m00 - m11);
        q[W]     = (m10 - m01) / s;
        q[X]     = (m02 + m20) / s;
        q[Y]     = (m12 + m21) / s;
        q[Z]     = 0.25f * s;
    }

    return q;
}
