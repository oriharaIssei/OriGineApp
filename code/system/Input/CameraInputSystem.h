#pragma once

/// parent
#include "system/ISystem.h"

/// engine
class Input;

class CameraInputSystem
    : public ISystem {
public:
    CameraInputSystem() : ISystem(SystemCategory::Input) {}
    ~CameraInputSystem() = default;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity);

protected:
    Input* input_ = nullptr;
};
