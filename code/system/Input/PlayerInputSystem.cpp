#include "PlayerInputSystem.h"

/// engine
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"

#include "input/Input.h"

/// component
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"

void PlayerInputSystem::Initialize() {
    input_ = Input::getInstance();
}

void PlayerInputSystem::Finalize() {
    input_ = nullptr;
}

void PlayerInputSystem::UpdateEntity(GameEntity* _entity) {
    PlayerInput* playerInput   = getComponent<PlayerInput>(_entity);
    PlayerStatus* playerStatus = getComponent<PlayerStatus>(_entity);

    // ゲームパッドか,キーボード 片方だけ 入力
    if (input_->isPadActive()) {
        // 移動
        playerInput->setInputDirection(input_->getLStickVelocity().normalize());

        // ジャンプ
        /// 一度ジャンプ入力を検知したら,
        // ジャンプボタンが押されている間 PlayerInput JumpInputTime を加算し,
        // ジャンプボタンが離されたら JumpInput を false にする
        if (playerInput->isJumpInput()) {
            // ジャンプ状態でない場合は、ジャンプ入力を継続しない
            if (playerStatus->getState() != PlayerMoveState::JUMP) {
                playerInput->setJumpInput(false);
                playerInput->setJumpInputTime(0.0f);

            } else {
                bool isJumpButtonPressed = false;
                for (auto button : playerInput->getJumpButton()) {
                    if (input_->isPressButton(button)) {
                        isJumpButtonPressed = true;
                        break;
                    }
                }

                // ジャンプボタンが押されている場合は、ジャンプ入力を継続
                if (isJumpButtonPressed) {
                    playerInput->setJumpInput(true);
                    // ジャンプ入力時間を更新
                    playerInput->setJumpInputTime(
                        (std::min)(playerInput->getJumpInputTime() + getMainDeltaTime(), playerInput->getMaxJumpTime()));
                } else {
                    playerInput->setJumpInput(false);
                    playerInput->setJumpInputTime(0.0f);
                }
            }
        } else {
            for (auto button : playerInput->getJumpButton()) {
                if (input_->isTriggerButton(button)) {
                    playerInput->setJumpInput(true);
                    playerInput->setJumpInputTime(0.0f); // ジャンプ入力時間をリセット
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
            if (input_->isPressKey(key)) {
                front = true;
                break;
            }
        }
        for (auto key : playerInput->getMoveBackKeys()) {
            if (input_->isPressKey(key)) {
                back = true;
                break;
            }
        }
        for (auto key : playerInput->getMoveLeftKeys()) {
            if (input_->isPressKey(key)) {
                left = true;
                break;
            }
        }
        for (auto key : playerInput->getMoveRightKeys()) {
            if (input_->isPressKey(key)) {
                right = true;
                break;
            }
        }

        playerInput->setInputDirection(Vec2f(float(right - left), float(front - back)));

        // ジャンプ
        if (playerInput->isJumpInput()) {
            // ジャンプ状態でない場合は、ジャンプ入力を継続しない
            if (playerStatus->getState() != PlayerMoveState::JUMP) {
                playerInput->setJumpInput(false);
                playerInput->setJumpInputTime(0.0f);

            } else {
                bool isJumpButtonPressed = false;
                for (auto key : playerInput->getJumpKeys()) {
                    if (input_->isPressKey(key)) {
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
                if (input_->isTriggerKey(key)) {
                    playerInput->setJumpInput(true);
                    break;
                }
            }
        }
    }
}
