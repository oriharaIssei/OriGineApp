#pragma once
#include "system/ISystem.h"

static float clearTime = 0.0f;

class GetClearTime
    : public ISystem {
public:
    GetClearTime();
    ~GetClearTime() override;
    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(GameEntity* _entity) override;
};
