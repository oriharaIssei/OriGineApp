#pragma once
#include "system/ISystem.h"

class GetClearTime
    : public ISystem {
public:
    GetClearTime();
    ~GetClearTime() override;
    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
