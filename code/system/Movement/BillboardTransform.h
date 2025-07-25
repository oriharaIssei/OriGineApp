#pragma once
#include "system/ISystem.h"

class BillboardTransform
    : public ISystem {
public:
    BillboardTransform() : ISystem(SystemType::Movement, 0) {};
    ~BillboardTransform() override {}

    void Initialize();
    void Finalize();

private:
    void UpdateEntity(GameEntity* _entity) override;
};
