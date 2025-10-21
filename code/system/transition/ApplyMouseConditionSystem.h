#pragma once
#include "system/ISystem.h"

/// <summary>
/// MouseConditionを適用するシステム
/// </summary>
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
