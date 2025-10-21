#pragma once
#include "system/ISystem.h"

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
