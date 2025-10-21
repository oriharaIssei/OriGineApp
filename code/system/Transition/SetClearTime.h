#pragma once
#include "system/ISystem.h"

/// <summary>
/// ステージをクリアした時間を設定するシステム
/// </summary>
class SetClearTime
    : public ISystem {
public:
    SetClearTime();
    ~SetClearTime() override;
    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
