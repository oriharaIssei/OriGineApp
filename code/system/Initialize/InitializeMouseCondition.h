#pragma once
#include "system/ISystem.h"

class InitializeMouseCondition
    : public OriGine::ISystem {
public:
    InitializeMouseCondition();
    ~InitializeMouseCondition() override = default;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::EntityHandle _handle) override;
};
