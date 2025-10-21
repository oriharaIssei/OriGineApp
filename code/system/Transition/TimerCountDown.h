#pragma once

#include "system/ISystem.h"

/// <summary>
/// TimerComponentのカウントダウンを行うシステム
/// </summary>
class TimerCountDown
    : public ISystem {
public:
    TimerCountDown();
    ~TimerCountDown() override;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
