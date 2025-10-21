#pragma once
#include "system/ISystem.h"

class BillboardTransform
    : public ISystem {
public:
    BillboardTransform() : ISystem(SystemCategory::Movement, 0) {};
    ~BillboardTransform() override {}

    void Initialize();
    void Finalize();

private:
    void UpdateEntity(Entity* _entity) override;
};
