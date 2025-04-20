// PillarSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// PillerPhysicsSystem
///====================================================================
class FloatingFloorDamageSystem : public ISystem {
public:
    FloatingFloorDamageSystem();
    ~FloatingFloorDamageSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
