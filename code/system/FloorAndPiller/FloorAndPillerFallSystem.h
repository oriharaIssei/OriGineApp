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
class FloorAndPillerFallSystem : public ISystem {
private:
  
public:
    FloorAndPillerFallSystem();
    ~FloorAndPillerFallSystem();

    void Initialize() override;
    void Finalize() override;

    float Lerp(const float& start, const float& end, float t);

  
protected:
    void UpdateEntity(GameEntity* _entity) override;
};
