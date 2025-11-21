#pragma once

#include "system/ISystem.h"

/// <summary>
/// ゴールシェルオブジェクトの初期化をするクラス
/// </summary>
class GoalShellEffectInitialize
    : public ISystem {
public:
    GoalShellEffectInitialize();
    ~GoalShellEffectInitialize() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* entity) override;
};
