#include "PlayerInputSystem.h"

/// engine
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"
// input
#include "input/GamePadInput.h"
#include "input/KeyboardInput.h"

/// component
#include "component/Player/PlayerInput.h"
#include "component/Player/State/PlayerState.h"

void PlayerInputSystem::Initialize() {}
void PlayerInputSystem::Finalize() {}

void PlayerInputSystem::UpdateEntity(Entity* _entity) {
    KeyboardInput* keyInput = getScene()->getKeyboardInput();
    GamePadInput* padInput  = getScene()->getGamePadInput();

    PlayerInput* playerInput = getComponent<PlayerInput>(_entity);
    PlayerState* state       = getComponent<PlayerState>(_entity);

    // ゲームパッドか,キーボード 片方だけ 入力
    if (padInput->isActive()) {
        // 移動
        playerInput->setInputDirection(padInput->getLStick().normalize());

        // ジャンプ
        /// 一度ジャンプ入力を検知したら,
        // ジャンプボタンが押されている間 PlayerInput JumpInputTime を加算し,
        // ジャンプボタンが離されたら JumpInput を false にする
        if (playerInput->isJumpInput()) {
            // ジャンプ状態でない場合は、ジャンプ入力を継続しない
            if (state->getStateEnum() != PlayerMoveState::JUMP) {
                playerInput->setJumpInput(false);
                playerInput->setJumpInputTime(0.0f);
            } else {
                bool isJumpButtonPressed = false;
                for (auto button : playerInput->getJumpButton()) {
                    if (padInput->isPress(button)) {
                        isJumpButtonPressed = true;
                        break;
                    }
                }

                // ジャンプボタンが押されている場合は、ジャンプ入力を継続
                if (isJumpButtonPressed) {
                    playerInput->setJumpInput(true);
                    // ジャンプ入力時間を更新
                    playerInput->setJumpInputTime(playerInput->getJumpInputTime() + getMainDeltaTime());
                    if (playerInput->getJumpInputTime() >= playerInput->getMaxJumpTime()) {
                        playerInput->setJumpInput(false);
                        playerInput->setJumpInputTime(0.0f);
                    }
                } else {
                    playerInput->setJumpInput(false);
                    playerInput->setJumpInputTime(0.0f);
                }
            }
        } else {
            for (auto button : playerInput->getJumpButton()) {
                if (padInput->isPress(button)) {
                    playerInput->setJumpInput(true);
                    break;
                }
            }
        }
    } else {
        bool front = false;
        bool back  = false;
        bool left  = false;
        bool right = false;

        for (auto key : playerInput->getMoveFrontKeys()) {
            if (keyInput->isPress(key)) {
                front = true;
                break;
            }
        }
        for (auto key : playerInput->getMoveBackKeys()) {
            if (keyInput->isPress(key)) {
                back = true;
                break;
            }
        }
        for (auto key : playerInput->getMoveLeftKeys()) {
            if (keyInput->isPress(key)) {
                left = true;
                break;
            }
        }
        for (auto key : playerInput->getMoveRightKeys()) {
            if (keyInput->isPress(key)) {
                right = true;
                break;
            }
        }

        playerInput->setInputDirection(Vec2f(float(right - left), float(front - back)));

        // ジャンプ
        if (playerInput->isJumpInput()) {
            // ジャンプ状態でない場合は、ジャンプ入力を継続しない
            if (state->getStateEnum() != PlayerMoveState::JUMP) {
                playerInput->setJumpInput(false);
                playerInput->setJumpInputTime(0.0f);

            } else {
                bool isJumpButtonPressed = false;
                for (auto key : playerInput->getJumpKeys()) {
                    if (keyInput->isPress(key)) {
                        isJumpButtonPressed = true;
                        break;
                    }
                }

                // ジャンプボタンが押されている場合は、ジャンプ入力を継続
                if (isJumpButtonPressed) {
                    playerInput->setJumpInput(true);
                    // ジャンプ入力時間を更新
                    playerInput->setJumpInputTime(playerInput->getJumpInputTime() + getMainDeltaTime());
                    if (playerInput->getJumpInputTime() >= playerInput->getMaxJumpTime()) {
                        playerInput->setJumpInput(false);
                        playerInput->setJumpInputTime(0.0f);
                    }
                } else {
                    playerInput->setJumpInput(false);
                    playerInput->setJumpInputTime(0.0f);
                }
            }
        } else {
            for (auto key : playerInput->getJumpKeys()) {
                if (keyInput->isTrigger(key)) {
                    playerInput->setJumpInput(true);
                    break;
                }
            }
        }
    }
}
