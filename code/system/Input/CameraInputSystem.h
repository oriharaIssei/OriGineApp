#pragma once

/// parent
#include "system/ISystem.h"

/// enigne
// input
class GamepadInput;
class MouseInput;

/// ECS
// component
class CameraController;

/// <summary>
/// Cameraの入力処理を行うシステム
/// </summary>
class CameraInputSystem
    : public ISystem {
public:
    CameraInputSystem() : ISystem(SystemCategory::Input) {}
    ~CameraInputSystem() = default;

    void Initialize() override;
    void Finalize() override;

    void UpdateEntity(Entity* _entity);
    void InputUpdate(float _deltaTime, MouseInput* _mouseInput, GamepadInput* _padInput, CameraController* _cameraController);

protected:
};
