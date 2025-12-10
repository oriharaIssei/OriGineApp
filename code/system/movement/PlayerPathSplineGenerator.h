#pragma once
#include "system/ISystem.h"

class PlayerPathSplineGenerator
    : public OriGine::ISystem {
public:
    PlayerPathSplineGenerator();
    ~PlayerPathSplineGenerator() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(OriGine::Entity* _entity) override;
};
