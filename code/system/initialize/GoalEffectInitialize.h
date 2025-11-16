#pragma once

#include "system/ISystem.h"

/// <summary>
/// ゴールオブジェクトの初期化をするクラス
/// </summary>
class GoalEffectInitialize
    : public ISystem {
public:
    GoalEffectInitialize();
    ~GoalEffectInitialize() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* entity) override;
};
