// PillarSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// PillerPhysicsSystem
///====================================================================
class FloorUpdateMatrixSystem : public ISystem {
public:
    FloorUpdateMatrixSystem();
    ~FloorUpdateMatrixSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
