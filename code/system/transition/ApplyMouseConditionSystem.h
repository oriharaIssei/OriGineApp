#pragma once
#include "system/ISystem.h"

/// <summary>
/// MouseConditionを適用するシステム
/// </summary>
class ApplyMouseConditionSystem
    : public OriGine::ISystem {
public:
    ApplyMouseConditionSystem();
    ~ApplyMouseConditionSystem() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(OriGine::Entity* _entity) override;
};
