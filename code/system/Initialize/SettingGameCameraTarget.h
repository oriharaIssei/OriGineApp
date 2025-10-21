#pragma once

/// parent
#include "system/ISystem.h"

/// <summary>
/// ゲームカメラのターゲット設定を行うシステム
/// </summary>
class SettingGameCameraTarget
    : public ISystem {
public:
    SettingGameCameraTarget() : ISystem(SystemCategory::Initialize) {}
    ~SettingGameCameraTarget() = default;

    void Initialize() override;

    void Update() override;

    void Finalize() override;
};
