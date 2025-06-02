#pragma once
// FloorSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// FloorSystem
///====================================================================
class FloatingFloorSpawner;
class FloorStates;
class PillerStates;
class FloatingFloorRevivalSystem : public ISystem {
private:
  
public:
    FloatingFloorRevivalSystem();
    ~FloatingFloorRevivalSystem();

    void Initialize() override;
    void Finalize() override;

   
  
protected:
    void UpdateEntity(GameEntity* _entity) override;
};
