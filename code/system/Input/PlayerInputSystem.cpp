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
#include "component/player/PlayerInputDevice.h"
#include "component/player/state/PlayerState.h"

void PlayerInputSystem::Initialize() {}
void PlayerInputSystem::Finalize() {}

void PlayerInputSystem::UpdateEntity(Entity* _entity) {
    auto keyInput = GetScene()->GetKeyboardInput();
    auto padInput = GetScene()->GetGamePadInput();

    auto playerInput = GetComponent<PlayerInput>(_entity);
    auto state       = GetComponent<PlayerState>(_entity);

    GamePadInputDevice padDevice(padInput, playerInput);
    KeyboardInputDevice keyDevice(keyInput, playerInput);

    IPlayerInputDevice* device = SelectActiveDevice(&padDevice, &keyDevice);

    // --- 移動 ---
    playerInput->SetInputDirection(device->GetMoveDirection());

    // --- ジャンプ ---
    HandleJump(playerInput, state, device);
}

IPlayerInputDevice* PlayerInputSystem::SelectActiveDevice(IPlayerInputDevice* _padDevice, IPlayerInputDevice* _keyDevice) {
    if (_padDevice->IsActive()) {
        return _padDevice;
    }
    return _keyDevice;
}

void PlayerInputSystem::HandleJump(
    PlayerInput* input,
    PlayerState* state,
    IPlayerInputDevice* device) {

    // wallJumpは 常にTrigger
    input->SetWallJumpInput(false);

    if (input->IsJumpInput()) {
        // JUMP 状態以外は押しっぱなし無効
        if (state->GetStateEnum() != PlayerMoveState::JUMP) {
            input->SetJumpInput(false);
            input->SetJumpInputTime(0.0f);
            return;
        }

        // 押され続けている
        if (device->IsJumpPress()) {
            input->SetJumpInput(true);
            input->SetJumpInputTime(
                input->GetJumpInputTime() + GetMainDeltaTime());

            if (input->GetJumpInputTime() >= input->GetMaxJumpTime()) {
                input->SetJumpInput(false);
                input->SetJumpInputTime(0.0f);
            }
        } else {
            input->SetJumpInput(false);
            input->SetJumpInputTime(0.0f);
        }

    } else {
        // 最初の押し
        if (device->IsJumpTrigger()) {
            input->SetJumpInput(true);

            // wallJump 判定
            input->SetWallJumpInput(state->GetStateEnum() == PlayerMoveState::WALL_RUN);
        }
    }
}
