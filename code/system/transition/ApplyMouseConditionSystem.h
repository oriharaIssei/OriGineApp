#pragma once
#include "system/ISystem.h"

class ApplyMouseConditionSystem
    : public ISystem {
public:
    ApplyMouseConditionSystem();
    ~ApplyMouseConditionSystem() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
