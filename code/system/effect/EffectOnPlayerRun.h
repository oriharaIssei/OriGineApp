#pragma once
#include "system/ISystem.h"

/// <summary>
/// Playerが走っているときにエフェクトを発生させるシステム
/// </summary>
class EffectOnPlayerRun
    : public ISystem {
public:
    EffectOnPlayerRun();
    ~EffectOnPlayerRun();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* entity) override;
private:
    const float maxIntensity = 3.8f;
};
