#pragma once
#include "system/ISystem.h"

class CreateMeshFromSpline
    : public ISystem {
public:
    CreateMeshFromSpline();
    ~CreateMeshFromSpline() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
