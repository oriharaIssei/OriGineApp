#pragma once
#include "system/ISystem.h"

/// engine
// input
class KeyboardInput;
class GamepadInput;

/// ECS
// component
class IPlayerInputDevice;
class PlayerInput;
class PlayerState;

/// <summary>
/// PlayerのInputを処理するシステム
/// </summary>
class PlayerInputSystem
    : public ISystem {
public:
    PlayerInputSystem() : ISystem(SystemCategory::Input) {}
    ~PlayerInputSystem() {}

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

    void UpdateEntity(Entity* _entity) override;
    void InputUpdate(float _deltaTime, KeyboardInput* _keyInput, GamepadInput* _padInput, PlayerInput* _playerInput, PlayerState* _playerState);

private:
    IPlayerInputDevice* SelectActiveDevice(IPlayerInputDevice* _padDevice, IPlayerInputDevice* _keyDevice);
    void HandleJump(float _deltaTime, PlayerInput* _playerInput, PlayerState* _state, IPlayerInputDevice* _device);
};
