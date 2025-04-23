#pragma once

/// parent
#include "system/ISystem.h"

class SettingGameCameraTarget
    : public ISystem {
public:
    SettingGameCameraTarget() : ISystem(SystemType::Initialize) {}
    ~SettingGameCameraTarget() = default;

    void Initialize() override;

    void Update() override;

    void Finalize() override;
};
