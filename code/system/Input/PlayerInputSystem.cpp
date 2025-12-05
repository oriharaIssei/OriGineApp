#include "PlayerInputSystem.h"

/// engine
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"
// input
#include "input/GamePadInput.h"
#include "input/KeyboardInput.h"

/// component
#include "component/player/PlayerInput.h"
#include "component/player/state/PlayerState.h"

void PlayerInputSystem::Initialize() {}
void PlayerInputSystem::Finalize() {}

void PlayerInputSystem::UpdateEntity(Entity* _entity) {
    KeyboardInput* keyInput = GetScene()->GetKeyboardInput();
    GamePadInput* padInput  = GetScene()->GetGamePadInput();

    PlayerInput* playerInput = GetComponent<PlayerInput>(_entity);
    PlayerState* state       = GetComponent<PlayerState>(_entity);

    // ゲームパッドか,キーボード 片方だけ 入力
    if (padInput->IsActive()) {
        // 移動
        playerInput->SetInputDirection(padInput->GetLStick().normalize());

        // ジャンプ
        /// 一度ジャンプ入力を検知したら,
        // ジャンプボタンが押されている間 PlayerInput JumpInputTime を加算し,
        // ジャンプボタンが離されたら JumpInput を false にする
        if (playerInput->IsJumpInput()) {
            if (state->GetStateEnum() == PlayerMoveState::WALL_RUN) {

                playerInput->SetJumpInput(false);
                for (auto button : playerInput->GetJumpButton()) {
                    if (padInput->IsTrigger(button)) {
                        playerInput->SetJumpInput(true);
                        break;
                    }
                }
            }

            // ジャンプ状態でない場合は、ジャンプ入力を継続しない
            if (state->GetStateEnum() != PlayerMoveState::JUMP) {
                playerInput->SetJumpInput(false);
                playerInput->SetJumpInputTime(0.0f);
            } else {
                bool isJumpButtonPressed = false;
                for (auto button : playerInput->GetJumpButton()) {
                    if (padInput->IsPress(button)) {
                        isJumpButtonPressed = true;
                        break;
                    }
                }

                // ジャンプボタンが押されている場合は、ジャンプ入力を継続
                if (isJumpButtonPressed) {
                    playerInput->SetJumpInput(true);
                    // ジャンプ入力時間を更新
                    playerInput->SetJumpInputTime(playerInput->GetJumpInputTime() + GetMainDeltaTime());
                    if (playerInput->GetJumpInputTime() >= playerInput->GetMaxJumpTime()) {
                        playerInput->SetJumpInput(false);
                        playerInput->SetJumpInputTime(0.0f);
                    }
                } else {
                    playerInput->SetJumpInput(false);
                    playerInput->SetJumpInputTime(0.0f);
                }
            }
        } else {
            for (auto button : playerInput->GetJumpButton()) {
                if (padInput->IsPress(button)) {
                    playerInput->SetJumpInput(true);
                    break;
                }
            }
        }
    } else {
        bool front = false;
        bool back  = false;
        bool left  = false;
        bool right = false;

        for (auto key : playerInput->GetMoveFrontKeys()) {
            if (keyInput->IsPress(key)) {
                front = true;
                break;
            }
        }
        for (auto key : playerInput->GetMoveBackKeys()) {
            if (keyInput->IsPress(key)) {
                back = true;
                break;
            }
        }
        for (auto key : playerInput->GetMoveLeftKeys()) {
            if (keyInput->IsPress(key)) {
                left = true;
                break;
            }
        }
        for (auto key : playerInput->GetMoveRightKeys()) {
            if (keyInput->IsPress(key)) {
                right = true;
                break;
            }
        }

        playerInput->SetInputDirection(Vec2f(float(right - left), float(front - back)));

        // ジャンプ
        if (playerInput->IsJumpInput()) {
            // ジャンプ状態でない場合は、ジャンプ入力を継続しない
            if (state->GetStateEnum() != PlayerMoveState::JUMP) {
                playerInput->SetJumpInput(false);
                playerInput->SetJumpInputTime(0.0f);

            } else {
                bool isJumpButtonPressed = false;
                for (auto key : playerInput->GetJumpKeys()) {
                    if (keyInput->IsPress(key)) {
                        isJumpButtonPressed = true;
                        break;
                    }
                }

                // ジャンプボタンが押されている場合は、ジャンプ入力を継続
                if (isJumpButtonPressed) {
                    playerInput->SetJumpInput(true);
                    // ジャンプ入力時間を更新
                    playerInput->SetJumpInputTime(playerInput->GetJumpInputTime() + GetMainDeltaTime());
                    if (playerInput->GetJumpInputTime() >= playerInput->GetMaxJumpTime()) {
                        playerInput->SetJumpInput(false);
                        playerInput->SetJumpInputTime(0.0f);
                    }
                } else {
                    playerInput->SetJumpInput(false);
                    playerInput->SetJumpInputTime(0.0f);
                }
            }
        } else {
            for (auto key : playerInput->GetJumpKeys()) {
                if (keyInput->IsTrigger(key)) {
                    playerInput->SetJumpInput(true);
                    break;
                }
            }
        }
    }
}
