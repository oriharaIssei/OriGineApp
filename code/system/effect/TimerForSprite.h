#pragma once
#include "system/ISystem.h"

/// <summary>
/// Timerの数値をSpriteに適用するシステム
/// </summary>
class TimerForSprite
    : public ISystem {
public:
    TimerForSprite();
    ~TimerForSprite() override;

    virtual void Initialize();
    // virtual void Update();
    // virtual void Edit();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(Entity* _entity) override;
};
