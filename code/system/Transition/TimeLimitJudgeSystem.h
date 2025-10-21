#pragma once
#include "system/ISystem.h"

/// <summary>
/// 時間制限によるゲームオーバー判定を行うシステム
/// </summary>
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
