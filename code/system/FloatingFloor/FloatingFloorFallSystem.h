#pragma once
// FloorSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// FloorSystem
///====================================================================
class FloorAndPillerSpawner;
class FloorStates;
class PillerStates;
class FloatingFloorFallSystem : public ISystem {
private:
  
public:
    FloatingFloorFallSystem();
    ~FloatingFloorFallSystem();

    void Initialize() override;
    void Finalize() override;

    float Lerp(const float& start, const float& end, float t);

  
protected:
    void UpdateEntity(GameEntity* _entity) override;
};
