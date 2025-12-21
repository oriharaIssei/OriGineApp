#pragma once
#include "system/ISystem.h"

namespace OriGine {
/// engine
// input
class KeyboardInput;
class GamepadInput;
}

/// ECS
// component
class IPlayerInputDevice;
class PlayerInput;
class PlayerState;

/// <summary>
/// PlayerのInputを処理するシステム
/// </summary>
class PlayerInputSystem
    : public OriGine::ISystem {
public:
    PlayerInputSystem() : ISystem(OriGine::SystemCategory::Input) {}
    ~PlayerInputSystem() {}

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

    void UpdateEntity(OriGine::EntityHandle _handle) override;
    void InputUpdate(float _deltaTime, OriGine::KeyboardInput* _keyInput, OriGine::GamepadInput* _padInput, PlayerInput* _playerInput, PlayerState* _playerState);

private:
    IPlayerInputDevice* SelectActiveDevice(IPlayerInputDevice* _padDevice, IPlayerInputDevice* _keyDevice);
    void HandleJump(float _deltaTime, PlayerInput* _playerInput, PlayerState* _state, IPlayerInputDevice* _device);
};
