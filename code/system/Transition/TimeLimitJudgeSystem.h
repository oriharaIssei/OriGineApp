#pragma once
#include "system/ISystem.h"

class TimeLimitJudgeSystem
    : public ISystem {
public:
    TimeLimitJudgeSystem();
    ~TimeLimitJudgeSystem() override;
    void Finalize() override;
    void Initialize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
