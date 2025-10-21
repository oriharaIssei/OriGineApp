#pragma once

/// parent
#include "system/ISystem.h"

/// engine
class Input;

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

protected:
    void UpdateEntity(Entity* _entity);
};
