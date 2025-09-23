#pragma once
#include "system/ISystem.h"

class CreateStage
    : public ISystem {
public:
    CreateStage() : ISystem(SystemCategory::Initialize, 0) {}
    void Initialize() override {}
    void Finalize() override {}

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
