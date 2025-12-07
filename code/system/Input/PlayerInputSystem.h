#pragma once
#include "system/ISystem.h"

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

private:
    void UpdateEntity(Entity* _entity) override;

    IPlayerInputDevice* SelectActiveDevice(IPlayerInputDevice* _padDevice, IPlayerInputDevice* _keyDevice);
    void HandleJump(PlayerInput* _playerInput, PlayerState* _state, IPlayerInputDevice* _device);
};
