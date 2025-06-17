#include "PlayerInputSystem.h"

/// engine
#define ENGINE_ECS
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
    PlayerInput* playerInput = getComponent<PlayerInput>(_entity);

    // ゲームパッドか,キーボード 片方だけ 入力
    if (input_->isPadActive()) {
        // 移動
        playerInput->setInputDirection(input_->getLStickVelocity().normalize());

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
        playerInput->setJumpInput(false);
        for (auto key : playerInput->getJumpKeys()) {
            if (input_->isPressKey(key)) {
                playerInput->setJumpInput(true);
                break;
            }
        }
    }
}
