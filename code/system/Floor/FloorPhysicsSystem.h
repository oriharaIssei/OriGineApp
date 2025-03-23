// FloorSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// FloorSystem
///====================================================================
class FloorPhysicsSystem : public ISystem {
public:
    FloorPhysicsSystem();
    ~FloorPhysicsSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
