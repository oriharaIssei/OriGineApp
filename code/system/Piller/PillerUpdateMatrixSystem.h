// PillarSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// PillerPhysicsSystem
///====================================================================
class PillerUpdateMatrixSystem : public ISystem {
public:
    PillerUpdateMatrixSystem();
    ~PillerUpdateMatrixSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
