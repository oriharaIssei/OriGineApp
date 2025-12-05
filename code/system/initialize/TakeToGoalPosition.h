#pragma once
#include "system/ISystem.h"

/// <summary>
/// GoalPositionの位置に移動させるシステム
/// </summary>
class TakeToGoalPosition
    : public ISystem {
public:
    TakeToGoalPosition();
    ~TakeToGoalPosition() override;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
