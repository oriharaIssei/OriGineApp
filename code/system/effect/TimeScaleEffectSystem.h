#pragma once

#include "system/ISystem.h"

/// <summary>
/// タイムスケールエフェクトを処理するシステム
/// </summary>
class TimeScaleEffectSystem
    : public OriGine::ISystem {
public:
    TimeScaleEffectSystem();
    ~TimeScaleEffectSystem() override;
    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(OriGine::EntityHandle _handle) override;
};
