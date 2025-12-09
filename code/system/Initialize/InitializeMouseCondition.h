#pragma once
#include "system/ISystem.h"

class InitializeMouseCondition
    : public ISystem {
public:
    InitializeMouseCondition();
    ~InitializeMouseCondition() override = default;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
