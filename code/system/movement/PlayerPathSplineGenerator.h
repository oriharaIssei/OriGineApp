#pragma once
#include "system/ISystem.h"

class PlayerPathSplineGenerator
    : public ISystem {
public:
    PlayerPathSplineGenerator();
    ~PlayerPathSplineGenerator() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
